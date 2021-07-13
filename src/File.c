#include "File.h"
#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

int verifyFilePath(char* path) {
    return (fopen(path, "r") != NULL) ? true : false;
}

char* getString(FILE *stream) {
    char c;
    int bytes = 0, capacity = 50;
    char *buffer = malloc(capacity);

    while ((c = fgetc(stream)) != EOF && c != '\n' && c != '\r') {
        bytes++;

        if (bytes+1 >= capacity) {
            capacity = capacity * 2;
            buffer = realloc(buffer, capacity);
            if (buffer == NULL) return NULL; //realloc failed
        }

        buffer[bytes-1] = c;
    }

    if (c == '\r') {
        c = fgetc(stream);
        if (c != '\n') {
            ungetc(c, stream);
        }
    }
    
    if (bytes == 0) {
        if (c == EOF) {
            free(buffer);
            return NULL;
        } else {
            buffer = malloc(1);
        }
    }

    buffer[bytes] = '\0';
    return buffer;
}

char* dataFromFile(char *path) {
    if (verifyFilePath(path) == false) return NULL; 
    FILE *stream = fopen(path, "r");

    char *str;
    while ((str = getString(stream)) != NULL) {
        printf("%s\n", str);
        free(str);
    }
    return "asldjkf";
}

char* fileContents(char *path) {
    if (verifyFilePath(path) == false) {
        printf("unverified path");
        return NULL; 
    }

    FILE *stream = fopen(path, "r");
    if (stream == NULL) return NULL;

    int capacity = 200, i = 0;
    char *buffer = malloc(capacity);
    char c;

    while ((c = fgetc(stream)) != EOF) {
        if (c == '\n') {
            continue;
        }

        i++;

        if (i+1 >= capacity) {
            capacity = capacity * 2;
            buffer = realloc(buffer, capacity);
            if (buffer == NULL) return NULL; //realloc failed
        }

        // if (c != '\n') {
            buffer[i-1] = c;
        // }
    }

    buffer[i] = '\0';
    return buffer;
}