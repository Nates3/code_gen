// NOTE(nates): Includes everything in the base layer

#include "base_types.c"
#include "base_math.c"

#if defined(ARENA_OS)
#include "base_arena_os.c"
#else
#include "base_arena_malloc.c"
#endif

#include "base_string.c"