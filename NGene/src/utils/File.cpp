#include "File.h"
#include <Debug.h>
#include <cstdio>
#include <fstream>


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

std::string read_from_file(const char* filename) {
    auto file = fopen(filename, "rb");
    if (file == nullptr) {
        LOG("We couldn't open " << filename << " to read from");
    }
    else {
        auto content = std::string{};
        fseek(file, 0, SEEK_END);
        content.resize(ftell(file));
        rewind(file);
        fread(&content[0], 1, content.size(), file);
        fclose(file);
        return content;
    }
}
