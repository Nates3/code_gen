
#define ARENA_COMMIT_GRANULARITY KB(4)

#define ArenaImpl_Reserve(size) OS_ReserveMemory(size)
#define ArenaImpl_Commit(ptr, size) OS_CommitMemory(ptr, size)
#define ArenaImpl_Decommit(ptr, size) OS_DecommitMemory(ptr, size);
#define ArenaImpl_Release(ptr) OS_ReleaseMemory(ptr)

func_ Arena *
ArenaAlloc(U64 size_to_reserve)
{
 size_to_reserve += sizeof(Arena);
 
 Arena *result = 0;
 void *memory = ArenaImpl_Reserve(size_to_reserve);
 U64 initial_commit_size = ARENA_COMMIT_GRANULARITY;
 ArenaImpl_Commit(memory, initial_commit_size);
 result = (Arena *)memory;
 
 result->commit_pos = initial_commit_size;
 result->pos = sizeof(Arena);
 result->size = size_to_reserve;
 result->align = 8;
 return(result);
}

func_ void *
ArenaPush(Arena *arena, U64 size)
{
 void *result = 0;
 U64 new_pos = arena->pos + size;
 if(new_pos <= arena->size) {
  U8 *base = (U8*)arena;
  U64 align = AlignUpNum(arena->pos, arena->align);
  result = base + arena->pos + align;
  arena->pos += size + align;
  
  if(arena->commit_pos < arena->pos) {
   U64 size_to_commit = arena->pos - arena->commit_pos;
   size_to_commit = AlignUp(size_to_commit, ARENA_COMMIT_GRANULARITY);
   size_to_commit = ClampTop(size_to_commit, arena->size);
   ArenaImpl_Commit(base + arena->commit_pos, size_to_commit);
   arena->commit_pos += size_to_commit;
  }
 }
 else {
  InvalidPath;
 }
 
 return(result);
}

func_ void *
ArenaPushZero(Arena *arena, U64 size)
{
 void *memory = PushSize(arena, size);
 MemoryZero(memory, size);
 return(memory);
}

func_ void
ArenaPop(Arena *arena, U64 size)
{
 if(arena->pos > size) {
  U64 pos = arena->pos - size;
  arena->pos = pos;
  
  U64 round_up_pos = AlignUp(arena->pos, ARENA_COMMIT_GRANULARITY);
  U64 decommit_size = arena->commit_pos - round_up_pos;
  if(decommit_size) {
   Assert(decommit_size%ARENA_COMMIT_GRANULARITY == 0);
   arena->commit_pos = round_up_pos;
   
   void *ptr = (U8 *)arena + round_up_pos;
   ArenaImpl_Decommit(ptr, decommit_size);
  }
 }
 else {
  InvalidPath;
 }
}

func_ void
ArenaSetPos(Arena *arena, U64 pos)
{
 if(pos < arena->pos) {
  U64 size = arena->pos - pos;
  ArenaPop(arena, size);
 }
}

func_ void
ArenaClear(Arena *arena)
{
 U64 size = arena->pos - sizeof(Arena);
 ArenaPop(arena, size);
}

func_ void
ArenaRelease(Arena *arena)
{
 ArenaImpl_Release(arena);
}

func_ ArenaTemp
BeginArenaTemp(Arena *arena)
{
 ArenaTemp result = {
  arena, arena->pos,
 };
 return(result);
}

func_ void
EndArenaTemp(ArenaTemp temp)
{
 ArenaSetPos(temp.arena, temp.pos);
}