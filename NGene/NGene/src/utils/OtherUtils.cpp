#include "OtherUtils.h"
#include <cstdlib>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
void pressToContinue() noexcept  { system("pause"); }
#else
void pressToContinue() noexcept  { system("read"); }
#endif