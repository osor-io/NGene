#pragma once
#include <iostream>

#ifdef _DEBUG

#define LOG(x) std::cout << x << std::endl;
#define LOG_NAMED(x) std::cout << #x << ": " << x << std::endl;
#define LOG_PTR(x) \
  std::cout << #x << ": " << static_cast<void*>(x) << std::endl;

#else

#define LOG(x)
#define LOG_NAMED(x)
#define LOG_PTR(x)

#endif
