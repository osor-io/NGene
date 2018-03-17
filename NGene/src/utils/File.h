#pragma once

#include <string>

bool file_exists(const char* filename);

void write_to_file(const char* filename, const char* content);

std::string read_from_file(const char* filename);

