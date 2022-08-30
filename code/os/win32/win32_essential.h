/* date = August 21st 2022 10:22 am */
#ifndef WIN32_ESSENTIAL_H
#define WIN32_ESSENTIAL_H

///////////////////////////
// NOTE(nates): Windows includes 

#include <windows.h>

//////////////////////////////////
// NOTE(nates): Win32 file iter

typedef struct Win32_FileIter
{
  HANDLE handle;
  WIN32_FIND_DATAW find_data;
  B32 done;
} Win32_FileIter;

StaticAssert(sizeof(Win32_FileIter) <= sizeof(OS_FileIter), win32_fileiter);

/////////////////////////////////////////////////////////////
// NOTE(nates): Special winmain init function

func_ void Win32_Main_Init(OS_ThreadContext *tctx_memory,
                           HINSTANCE hInstance, HINSTANCE hPrevInstance,
                           LPSTR lpCmdLine, int nCmdShow);

////////////////////////////////////////////////////////////////////
// NOTE(nates): Win32 time helpers
func_ FileProperties 
Win32_FilePropertiesFromFileData(DWORD file_attributes, FILETIME creation_time, 
                                 FILETIME last_write_time, DWORD file_size_high, 
                                 DWORD file_size_low);
func_ DateTime   Win32_DateTimeFromSystemTime(SYSTEMTIME *time);
func_ SYSTEMTIME Win32_SystemTimeFromDateTime(DateTime *time);
func_ DateTime   Win32_DateTimeFromFileTime(FILETIME *filetime);
func_ DenseTime  Win32_DenseTimeFromFileTime(FILETIME *filetime);

#endif //WIN32_ESSENTIAL_H
