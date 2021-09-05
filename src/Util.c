#include <time.h>
#include <stdio.h>
#include "Util.h"
#include "String.h"

char* curr_time() {
    time_t raw_time;
    struct tm *timeinfo;
    time(&raw_time);
    timeinfo = localtime(&raw_time);
    char *res = asctime(timeinfo);
    res[stringlen(res) - 1] = '\0';
    return res;
}