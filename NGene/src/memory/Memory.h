#pragma once

#include <cstdlib>

void* operator new(std::size_t count);

void operator delete(void* ptr);
