#include "Debug.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
void press_to_continue() noexcept { system("pause"); }
#else
void press_to_continue() noexcept { system("read"); }
#endif