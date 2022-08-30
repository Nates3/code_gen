/* date = August 17th 2022 2:29 am */
#ifndef BASE_ARENA_H
#define BASE_ARENA_H

///////////////////////////////////
// NOTE(nates): Arenas

typedef struct Arena
{
 U64 pos;
 U64 commit_pos;
 U64 size;
 U64 align;
}Arena;

typedef struct ArenaTemp
{
 Arena *arena;
 U64 pos;
}ArenaTemp;


// IMPORTANT NOTE(nates): Arena's are implimented differently depending
// on whether or not you include os_inc.h. Without the os layer, 
// arena's use malloc and commit memory upfront. With the os layer, 
// ArenaAlloc doesn't commit up front and instead reserves address space.
// Subsequent alloctaions on arena's in the os implimentation will commit memory
// If you want you can ignore this and just include this .h and your own
// .c implimentation

/////////////////////////////////
// NOTE(nates): Arena functions

func_ Arena *ArenaAlloc(U64 size);
func_ void  *ArenaPush(Arena *arena, U64 size);
func_ void  *ArenaPushZero(Arena *arena, U64 size);
func_ void   ArenaPop(Arena *arena, U64 size);
func_ void   ArenaSetPos(Arena *arena, U64 pos);
func_ void   ArenaClear(Arena *arena);
func_ void   ArenaRelease(Arena *arena);
#define PushArray(arena, type, count) (type *)ArenaPush(arena, sizeof(type)*count)
#define PushArrayZero(arena, type, count) (type *)ArenaPushZero(arena, sizeof(type)*count);
#define PushSize(arena, size) ArenaPush(arena, size)
#define PushSizeZero(arena, size) ArenaPushZero(arena, size)

func_ ArenaTemp BeginArenaTemp(Arena *arena);
func_ void      EndArenaTemp(ArenaTemp temp);

#endif //BASE_ARENA_H
