#include "FileHelper.h"
#include <unistd.h>


int verifyFilePath(char* path) {
    return access(path, F_OK);
}