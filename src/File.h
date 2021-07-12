#include <stdio.h>

//returns 1 if file exists, 0 if it does not
int verifyFilePath(char* path);

//returns the string contents of the file path or null in case of an error
char* dataFromFile(char* path);

char* fileContents(char *path);