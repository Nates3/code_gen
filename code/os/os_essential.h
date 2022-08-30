/* date = August 17th 2022 6:31 am */
#ifndef OS_ESSENTIAL_H
#define OS_ESSENTIAL_H

///////////////////////////////////
// NOTE(nates): Thread context

#if !defined(TCTX_SCRATCH_POOL_CAP)
#define TCTX_SCRATCH_POOL_CAP 4
#endif

typedef struct OS_ThreadContext
{
 Arena *scratch_pool[TCTX_SCRATCH_POOL_CAP];
} OS_ThreadContext;


////////////////////////////
//~ NOTE(nates): File iterator

typedef struct OS_FileIter
{
 U8 v[640];
} OS_FileIter;

///////////////////////////////
// NOTE(nates): File paths

typedef U32 OS_SystemPath;
enum OS_SystemPath
{
 OS_SystemPath_CurrentDir,
 OS_SystemPath_ExeDir,
 OS_SystemPath_UserDir,
 OS_SystemPath_TempDir,
 OS_SystemPath_COUNT,
};

////////////////////////////
// NOTE(nates): DLLS

typedef U64 OS_Handle;

typedef struct OS_Library
{
 OS_Handle handle;
 DenseTime last_write_time;
} OS_Library;

///////////////////////////
//~ NOTE(nates): Setup

func_ void OS_Main_Init(OS_ThreadContext *tctx, int argc, char **args);
func_ String8List OS_GetCommandLineArgs(void);

///////////////////////////////////////////////
//~ NOTE(nates): Shutdown

func_ void OS_Abort(void);

///////////////////////////////
//~ NOTE(nates): Thread Context

func_ void OS_TctxSet(void *ptr);
func_ void *OS_TctxGet(void);

//////////////////////////
// NOTE(nates): Memory Functions

func_ void *OS_ReserveMemory(U64 size);
func_ void  OS_CommitMemory(void *ptr, U64 size);
func_ void  OS_DecommitMemory(void *ptr, U64 size);
func_ void  OS_ReleaseMemory(void *base);

////////////////////////////////
//~ NOTE(nates): OS_FileFunctions

func_ String8 OS_ReadFile(Arena *arena, String8 filename);
func_ B32     OS_WriteListFile(String8 filename, String8List data);

func_ FileProperties OS_FileProperties(String8 filename);

func_ B32 OS_PathExists(String8 path);
func_ B32 OS_DeleteFile(String8 filename);
func_ B32 OS_RenameFile(String8 filename, String8 newname);
func_ B32 OS_MakeDirectory(String8 path);
func_ B32 OS_DeleteDirectory(String8 path);

func_ OS_FileIter OS_FileIterInit(String8 path);
func_ B32  OS_FileIterNext(Arena *arena, OS_FileIter *iter,
                           String8 *name_out, FileProperties *prop_out);
func_ void OS_FileIterEnd(OS_FileIter *iter);

// NOTE(nates): System paths

func_ String8 OS_FilePath(Arena *arena, OS_SystemPath path);

/////////////////////////////////
//~ NOTE(nates): Time functions

func_ DateTime OS_GetUniversalTime(void);
func_ DateTime OS_LocalTimeFromUniversal(DateTime *time);
func_ DateTime OS_UniversalTimeFromLocalTime(DateTime *time);

// Percision

func_ U64 OS_GetMicroseconds(void);
func_ U64 OS_GetMilliseconds(void);
func_ void OS_SleepMS(U64 ms);


//////////////////////////////////////
//~ NOTE(nates): DLL's

func_ OS_Library OS_LoadLib(String8 filename);
func_ B32        OS_CloseLib(OS_Library library);
#define OS_GetLibProc(library, func_name, func_type) \
(func_type *)OS_GetLibProc_(library, func_name)
func_ void *     OS_GetLibProc_(OS_Library library, String8 function_name);

/////////////////////////////////////////
//~ NOTE(nates): Entropy

func_ void OS_GetEntropy(U64 *seed);


#endif // OS_ESSENTIAL_H
