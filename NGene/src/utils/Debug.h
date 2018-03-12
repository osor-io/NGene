#pragma once
#include <iostream>

/* Usefull to avoid any LOGs in the console in release mode
#ifndef _DEBUG
#define _NOLOG
#endif
*/

#ifndef _NOLOG

#define ENDL std::cout << std::endl;
#define LOG(x) std::cout << x << std::endl;
#define LOG_NAMED(x) std::cout << #x << ": " << x << std::endl;
#define LOG_PTR(x) \
  std::cout << #x << ": " << static_cast<void*>(x) << std::endl;

#else

#define ENDL
#define LOG(x)
#define LOG_NAMED(x)
#define LOG_PTR(x)

#endif

void pressToContinue() noexcept;
