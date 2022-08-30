
func_ Arena *
ArenaAlloc(U64 size)
{
 size += sizeof(Arena);
 void *memory = malloc(size);
 Arena *result = (Arena *)memory;
 result->pos = sizeof(Arena);
 result->commit_pos = 0;
 result->size = size;
 result->align = 8;
 return(result);
}

func_ void *
ArenaPush(Arena *arena, U64 size)
{
 void *memory = 0;
 U64 new_pos = arena->pos + size;
 if(new_pos < arena->size) {
  U64 align = AlignUpNum(new_pos, arena->align);
  new_pos += align;
  memory = (U8 *)arena + arena->pos;
  arena->pos = new_pos;
 }
 else {
  InvalidPath;
 }
 return(memory);
}

func_ void *
ArenaPushZero(Arena *arena, U64 size)
{
 void *memory = ArenaPush(arena, size);
 MemoryZero(memory, size);
}

func_ void
ArenaPop(Arena *arena, U64 size)
{
 U64 new_pos = ClampBot(sizeof(Arena), arena->pos - size);
 arena->pos = new_pos;
}

func_ void
ArenaSetPos(Arena *arena, U64 pos)
{
 arena->pos = pos;
}

func_ void
ArenaClear(Arena *arena)
{
 arena->pos = sizeof(Arena);
}

func_ void
ArenaRelease(Arena *arena)
{
 free(arena);
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