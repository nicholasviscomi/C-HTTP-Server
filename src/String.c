#include <stdlib.h>
#include <string.h>

#include "File.h"
#include "String.h"


size_t stringlen(const char *str) {
    const char *s;
    for (s = str; *s; ++s);
	return (unsigned int) (s - str);
}

char* concatenate(char *src, char *append) {
    char *res = malloc(stringlen(src) + stringlen(append) + 1); //make res size of both param + \0
    int ptr = 0, temp = 0;
    
    while(src[temp] != '\0') { 
        res[ptr++] = src[temp++];
    } //make res point to '\0' of src

    temp = 0;
    while(append[temp] != '\0'){
        res[ptr++] = append[temp++];
    } //one by one add each char from append to res

    return res;
}

//GET /test HTTP/1.1 ===> /test
char* parseQuery(const char *txt) {
    int i =  0;
    while (txt[i] != '/') {
        i++;
    } //get i to the start of the actualy request

    int j = 0, buffSize = 100;
    char *result = malloc(buffSize); 
    while(txt[i] != ' ') {
        if (i > buffSize) {
            result = realloc(result, buffSize * 2);
        }
        result[j++] = txt[i++];
    }

    if (strcmp(result, "/") == 0) {
        result = "/index.html";
    }

    return result;    
}

char* compileResponse(char *starter, char *query) {
    char *contents = fileContents(query, NULL);
    if (contents == NULL) return NULL;
    char *response = starter;

    char* contLen = malloc(20); 
    sprintf(contLen, "%lu", stringlen(contents)); //put content length into string variable

    response = concatenate(response, contLen); //add content length

    contents = concatenate("\n\n", contents); //add two new lines to format response correctly

    response = concatenate(response, contents); //add contents

    return response;
}

void strTrim(char *s, int trimLen, size_t len) {
    // s = realloc(s, len-trimLen);
    // s[len-trimLen] = '\0';
    return;
}