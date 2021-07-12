#include "String.h"
#include <stdlib.h>


size_t stringlen(const char *str) {
    const char *s;
    for (s = str; *s; ++s);
	return (unsigned int) (s - str);
}

char* concatenate(char *src, char *append) {
    char *res = malloc(stringlen(src) + stringlen(append) + 1); //make result size of both param + \0
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