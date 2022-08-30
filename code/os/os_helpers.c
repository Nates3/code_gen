
///////////////////////////////
// NOTE(nates): Thread context functions

func_ void 
OS_TctxInit(OS_ThreadContext *tctx)
{
 Arena **scratch = tctx->scratch_pool;
 for(U32 i = 0; i < ArrayCount(tctx->scratch_pool); ++i, scratch += 1)
 {
  *scratch = ArenaAlloc(GB(1));
 }
}

func_ Arena *
OS_TctxGetScratch(OS_ThreadContext *tctx, Arena **conflict_array, U32 count)
{
 Arena *result = 0;
 Arena **scratch = tctx->scratch_pool;
 for(U32 i = 0; i < ArrayCount(tctx->scratch_pool); ++i, scratch += 1)
 {
  B32 is_non_conflict = true;
  
  Arena **conflict_ptr = conflict_array;
  for(U32 j = 0; j < count; ++j, conflict_ptr += 1)
  {
   if(*scratch == *conflict_ptr)
   {
    is_non_conflict = false;
    break;
   }
  }
  
  if(is_non_conflict)
  {
   result = *scratch;
   break;
  }
 }
 return(result);
}

////////////////////////////
// NOTE(nates): Scratch helper functions

func_ ArenaTemp
GetScratch(Arena *conflict0, Arena *conflict1)
{
 OS_ThreadContext *tctx = (OS_ThreadContext *)OS_TctxGet();
 Arena *conflict_array[2] = {conflict0, conflict1};
 Arena *scratch = OS_TctxGetScratch(tctx, conflict_array, 2);
 return(BeginArenaTemp(scratch));
}


//////////////////////////////
// NOTE(nates): OS_File helpers

func_ B32
OS_WriteFile(String8 filename, String8 data)
{
 String8Node node = {0};
 String8List list = {0};
 PushExplicitStr8List(&list, data, &node);
 B32 result = OS_WriteListFile(filename, list);
 return(result);
}