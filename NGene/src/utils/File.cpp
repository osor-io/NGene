#define _CRT_SECURE_NO_WARNINGS //Because we have to use fopen here

#include "File.h"
#include <Debug.h>
#include <cstdio>


void writeToFile(const char * filename, const char * content) {

    FILE * file;
    file = fopen(filename, "w");
    if (file == nullptr) {
        LOG("We couldn't open " << filename << " to write to");
    }
    else {
        fputs(content, file);
        fclose(file);
    }

}
