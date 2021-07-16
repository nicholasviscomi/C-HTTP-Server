#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"
#include "File.h"
#include "String.h"

const char *connPath = "/Users/nickviscomi/Desktop/VSCode/C/HTTPServer/src/Database/Connection.json";
const char *numConnectionsPath = "/Users/nickviscomi/Desktop/VSCode/C/HTTPServer/src/Database/NumConnections.txt";

int logConnection(char *date, char *time, char *query, char *request) {
    FILE *fp = fopen(connPath, "r");
    if (fp == NULL) return 0;

    int contLen = 0;
    char *contents = fileContents(connPath, &contLen), c; 
    if (contents == NULL) return 0;
    int i = contLen-2; // subtract 2 is to get past the closing curly brace


    while ((c = contents[i]) != '}') {
        i--;
    } //make i the index of the '}' of the previous connection

    contents[++i] = ','; //add comma to end of previous logged connection
    contents[++i] = '\n';
    contents[++i] = '\n'; //makes blank line

    //format new connection
    int nConnections = getNumConnections(fp); 
    if (nConnections == 0) return NULL;
    fclose(fp); //no longer need to read from this file

    char *newConnection = malloc(100);
    sprintf(newConnection, "\"%d\" : {\n\t\"time\" : \"%s|%s\",\n\t\"query\" : \"%s\",\n\t\"request\" : \"%s\"\n}\n}", ++nConnections, date, time, query, request);
    size_t newConnLen = strlen(newConnection);
    contents = realloc(contents, (contLen + 2) + newConnLen + 1);
    if (contents == NULL) return 0;

    //add new connection to contents
    for (int j = 0; j < newConnLen; j++) {
        contents[++i] = newConnection[j];
    }

    //write contents to file
    fp = fopen(connPath, "w");
    if (fp == NULL) return 0;
    fprintf(fp, "%s", contents);
    fclose(fp);
    return 1;
}

unsigned int getNumConnections(FILE *fp) {
    char *contents = fileContents(numConnectionsPath, NULL);
    if (contents == NULL) return 0;

    int n = atoi(contents);
    if (n == 0) return 0;

    return n;
}

int setNumConnections() {
    char *contents = fileContents(numConnectionsPath, NULL);
    if (contents == NULL) return 0;

    int n = atoi(contents);
    if (n == 0) return 0;

    FILE *fp = fopen(numConnectionsPath, "w");
    if (fp == NULL) return 0;

    fprintf(fp, "%d", ++n);
    fclose(fp);
    return 1;
}