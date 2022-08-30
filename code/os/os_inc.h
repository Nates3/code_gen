/* date = August 20th 2022 9:26 am */

#ifndef OS_INC_H
#define OS_INC_H


//NOTE(nates): The OS layer overrides Arena implimentation to use
// reserve && commit style allocation using this define && affects "base_inc.c"
// you can ignore if this you want to include arena implimentation files 
// yourself
#define ARENA_OS

#include "os_essential.h"
#include "os_helpers.h"

#endif //OS_INC_H
