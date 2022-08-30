
#include "os_helpers.c"

#if OS_WINDOWS
#include "win32/win32_essential.h"
#include "win32/win32_essential.c"
#else
#error there is no backend implementation for os_essential.h for this operating system
#endif