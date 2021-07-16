#include <stdlib.h>

//returns length of string
size_t stringlen(const char *str);

//adds two strings together and returns full string
char* concatenate(char *src, char *append);

//takes HTTP GET request from web client and returns the clients request (i.e. /connections.json&password=123)
char* parseQuery(const char *txt);

//puts together HTTP response
char* compileResponse(char *starter, char *query);

//trims the end of a string
void strTrim(char *s, int trimLen, size_t len);