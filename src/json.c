#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"
#include "File.h"
#include "String.h"

const char *connPath = "/Users/nickviscomi/Desktop/VSCode/C/HTTPServer/src/Database/Connection.json";

int logConnection(char *date, char *time, char *query, char *request) {
    FILE *fp = fopen(connPath, "r");
    if (fp == NULL) return 0;

    int contLen = 0;
    char *contents = fileContents(connPath, &contLen), c; 
    int i = contLen-2; // subtract 2 is to get past the closing curly brace


    while ((c = contents[i]) != '}') {
        i--;
    } //make i the index of the '}' of the previous connection

    contents[++i] = ','; //add comma to end of previous logged connection
    contents[++i] = '\n';
    contents[++i] = '\n'; //makes blank line

    //format new connection
    int nConnections = getNumConnections(fp); fclose(fp); //no longer need to read from this file
    char *newConnection = malloc(100);
    
    sprintf(newConnection, "\"%d\" : {\n\t\"time\" : \"%s|%s\",\n\t\"query\" : \"%s\",\n\t\"request\" : \"%s\"\n}\n}", ++nConnections, date, time, query, request);
    size_t newConnLen = strlen(newConnection);
    contents = realloc(contents, (contLen + 2) + newConnLen + 1);

    //add new connection to contents
    for (int j = 0; j < newConnLen; j++) {
        contents[++i] = newConnection[j];
    }

    //write contents to file
    fp = fopen(connPath, "w");
    fprintf(fp, "%s", contents);
    fclose(fp);
    return 1;
}

unsigned int getNumConnections(FILE *fp) {
    char c;
    while ((c = fgetc(fp)) != ':') {
        if (c == EOF) return 0;
        continue;
    }//move fgetc to the index of the colon before "numconnections" tag

    while ((c = fgetc(fp)) != '\"' && c != EOF) {
        if (c == EOF) return 0;
        continue;
    }//move fgetc to the " right before the number
    
    int resI = 0;
    char* res = malloc(10);
    while ((c = fgetc(fp)) != '\"') {
        if (c == EOF) return 0;
        res[resI++] = c;
    }

    int intRes = atoi(res);
    return intRes;
}

int setNumConnections(char *contents, int len) {
    //go through array up to the number adding the characters to 
    // int nConn = getNumConnections(fp); 
    // char *res = malloc(len + 5);
    // char *fhalf = malloc(len), *shalf = malloc(len);
    // 
    // char c; int i = 0;
    // while ((c = contents[i]) != ':') {
    //     if (c == EOF) return 0;
    //     fhalf[i] = contents[i];
    //     i++;
    // }//move i to the colon before the num
    //
    // while ((c = contents[i]) != '\"') {
    //     if (c == EOF) return 0;
    //     fhalf[i] = contents[i];
    //     i++;
    // }//move i to the " right before the number
    //
    // printf("fhalf: %s\n", fhalf);
    // 
    // char *updatedNConn = NULL;
    // sprintf(updatedNConn, "%d", nConn + 1);
    // concatenate(contents, updatedNConn);
    char *fhalf = malloc(len), *shalf = malloc(len);
    char c; int i = 0;
    while ((c = contents[i]) != ':') {
        if (c == EOF) return 0;
        i++;
    }//move i to the colon before the num
    
    while ((c = contents[i]) != '\"') {
        if (c == EOF) return 0;
        i++;
    }//move i to the index before the " before the number
    // i++; //move i to the "

    //ERROR: when switching between 9 and 10 the second quote isn't put around the 10 causing an error; going from 10 to 11 is fine however
    //SOL: don't include the quotes in fhalf and shalf, instead wrap cn in quotes

    strncpy(fhalf, contents, i * sizeof(char));

    FILE *fp = fopen(connPath, "r");
    int n = getNumConnections(fp);
    char *cn = malloc(5);
    if (n == 9 || n == 99 || n == 999 || n == 9999 || n == 99999 || n == 999999 || n == 9999999 || n == 99999999 || n == 999999999) {
        n++;
        sprintf(cn, "\"%d\",", n);    
    } else {
        n++;
        sprintf(cn, "\"%d\"", n);
    }
    fclose(fp);

    strncpy(shalf, contents + (i + strlen(cn)), len - (i + strlen(cn)));
    printf("fhalf: %s\n", fhalf);
    printf("Shalf: %s\n", shalf);

    char *res = malloc(len + strlen(cn) + 1);
    res = concatenate(fhalf, cn); 
    //check if there is a quote at the end of the number

    printf("Res1: %s\n", res);
    res = concatenate(res, shalf);
    printf("Res2: %s\n", res);
    fp = fopen(connPath, "w");
    fprintf(fp, "%s", res);
    fclose(fp);

    return 1;
}