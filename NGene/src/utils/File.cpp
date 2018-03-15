#include "File.h"
#include <Debug.h>
#include <cstdio>


void write_to_file(const char * filename, const char * content) {

    auto file = fopen(filename, "w");
    if (file == nullptr) {
        LOG("We couldn't open " << filename << " to write to");
    }
    else {
        fputs(content, file);
        fclose(file);
    }

}
