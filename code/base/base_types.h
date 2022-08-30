/* date = July 4th 2022 4:48 pm */

#ifndef BASE_TYPES_H
#define BASE_TYPES_H

//////////////////////////////
// NOTE(nates): Context defines
#if defined(__clang__)
# define COMPILER_CLANG 1
# if defined(_WIN32)
#  define OS_WINDOWS 1
# elif defined(__gnu_linux__)
#  define OS_LINUX 1
# elif defined(__APPLE__) && defined(__MACH__)
#  define OS_MAC 1
# elif
#  error os not found for clang
# endif
#elif defined(_MSC_VER)
#define COMPILER_CL 1
# if defined(_WIN32)
#  define OS_WINDOWS 1
# else
#  error os not found for msvc 
# endif
#elif defined(__GNUC__)
# define COMPILER_GCC 1
# if defined(_WIN32)
#  define OS_WINDOWS 1
# elif defined(__gnu_linux__)
#  define OS_LINUX 1
# elif defined(__APPLE__) && defined(__MACH__)
#  define OS_MAC 1
# elif
#  error os not found for gcc
# endif
#else
# error compiler context wasn't found
#endif

#if defined(COMPILER_CL)
#if defined(_M_AMD64)
# define ARCH_X64 1
#elif defined(_M_I86)
# define ARCH_X86 1
#elif defined(_M_ARM)
# define ARCH_ARM 1
#else
#error architecture not found msvc
#endif
#endif

#if defined(COMPILER_CLANG)
#if defined(__amd64__)
# define ARCH_X64 1
#elif defined(__i386__)
# define ARCH_X86 1
#elif defined(__arm__)
# define ARCH_ARM 1
#elif defined(__aarch64__)
#define ARCH_ARM64 1
#else
#error architecture not found clang
#endif
#endif

#if defined(COMPILER_GCC)
#if defined(__amd64__)
# define ARCH_X64 1
#elif defined(__i386__)
# define ARCH_X86 1
#elif defined(__arm__)
# define ARCH_ARM 1
#elif defined(__aarch64__)
#define ARCH_ARM64 1
#else
#error architecture not found
#endif
#endif


#if !defined(COMPILER_CL)
# define COMPILER_CL 0
#endif

#if !defined(COMPILER_CLANG)
# define COMPILER_CLANG 0
#endif

#if !defined(COMPILER_GCC)
# define COMPILER_GCC 0
#endif

#if !defined(OS_WINDOWS)
# define OS_WINDOWS 0
#endif

#if !defined(OS_LINUX)
# define OS_LINUX 0
#endif

#if !defined(OS_MAC)
# define OS_MAC 0
#endif

#if !defined(ARCH_X64)
# define ARCH_X64 0
#endif

#if !defined(ARCH_X86)
# define ARCH_X86 0
#endif

#if !defined(ARCH_ARM)
# define ARCH_ARM 0
#endif

#if !defined(ARCH_ARM64)
# define ARCH_ARM64 0
#endif

#if !defined(ENABLE_ASSERT)
#define ENABLE_ASSERT 1
#endif

////////////////////////////////
// NOTE(nates): Fundamental types

#define true 1
#define false 0

#include <stdint.h>

typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef float F32;
typedef double F64;

typedef S8 B8;
typedef S16 B16;
typedef S32 B32;
typedef S64 B64;

typedef void Void_Func(void);


//////////////////////
// NOTE(nates): Helper macros

#define Stmnt(S) do{ S }while(0)

#if !defined(AssertBreak)
#define AssertBreak (*(int *)0 = 0)
#endif

#if ENABLE_ASSERT
#define Assert(C) Stmnt( if(!(C)){ AssertBreak; } )
#else
#define Assert(C)
#endif
#define InvalidPath Assert(!"Invalid Code Path")
#define Unimplemented InvalidPath
#define InvalidCase default: {InvalidPath;} break

#if COMPILER_CL
#define export_ __declspec(dllexport)
#else
#error export not defined on this compiler
#endif

#define KB(number) ((U64)(number) << 10)
#define MB(number) ((U64)(number) << 20)
#define GB(number) ((U64)(number) << 30)
#define TB(number) ((U64)(number) << 40)

#define Thousand(number) ((U64)(number)*1000)
#define Million(number)  ((U64)(number)*1000000llu)
#define Billion(number)  ((U64)(number)*1000000000llu)
#define Trillion(number) ((U64)(number)*1000000000000llu)

#define StaticAssert(c, l) typedef U8 Glue(l, __LINE__) [(c)?1:-1]

#define Stringify_(S) #S
#define Stringify(S) Stringify_(S)
#define Glue_(A, B) A##B
#define Glue(A, B) Glue_(A, B)

#define ArrayCount(A) (sizeof(A) / sizeof(*(A)))

#define IntAsPtr(P) (unsigned long long)((char *)P - (char *)0)
#define PtrAsInt(N) (void *)((char *)0 + (N))

#define StructMember(S, M) (((S*)0)->M)
#define OffsetOfMember(S, M) IntFromPtr(&StructMember(S, M))

#define AlignUp(num, gran) ((num)+((gran) - 1))-(((num)+((gran) - 1))%(gran))
#define AlignUpNum(num, gran) AlignUp(num, gran) - num
#define AlignDown(num, gran) (num)-((num)%(gran))
#define AlignDownNum(num, gran) (num)%(gran)

#define Minimum(A, B) (((A < B)) ? (A) : (B))
#define Maximum(A, B) (((A > B)) ? (A) : (B))
#define Clamp(A, X, B) (((X) < (A)) ? (A) : ((X) > (B)) ? (B) : (X))
#define ClampTop(A, B) Minimum(A, B)
#define ClampBot(A, B) Maximum(A, B)
#define Clamp01(X) Clamp(0.0f, X, 1.0f)
#define Clamp01R64(X) Clamp(0.0, X, 1.0)

#define global_ static
#define func_ static
#define local_ static

#include <string.h>

#define MemoryZero(p, size) memset((p), 0, (size))
#define MemoryZeroStruct(P) MemoryZero((P), sizeof(*(P)))
#define MemoryZeroArray(A) MemoryZero((A), sizeof(A))
#define MemoryZeroTyped(P, C) MemoryZero((P), sizeof(*(P))*(C))

#define MemoryMatch(A, B, Z) (memcmp((A), (B), (Z)) == 0)

#define MemoryCopy(D, S, Z) memmove((D), (S), (Z))
#define MemoryCopyStruct(D, S) MemoryCopy((D), (S), \
Minimum(sizeof(*(D)), sizeof(*(S))))
#define MemoryCopyArray(D, S) MemoryCopy((D), (S), Minimum(sizeof(D), sizeof(S)))
#define MemoryCopyTyped(D, S, C) MemoryCopy((D), (S), Minimum(sizeof(*(D)), (sizeof(*(S))*(C))))

#define RemoveFlags(value, flag) Stmnt(value &= (~flag);)
#define ClearFlags(value) Stmnt(value = 0;)
#define OnlyFlag(value, flag) ((value - flag) == 0)
#define OnlyFlags(value, flags) (((value) & (flags)) == flags)
#define HasFlags(value, flags) (((value) & (flags)) != 0)


/////////////////////////////
// NOTE(nates): Linked lists

#define DLLPushBackNP(f, l, n, next, prev) ((f)==0?\
((f)=(l)=(n), (n)->next=(n)->prev=0):\
((n)->prev=(l), (l)->next=(n), (l)=(n), (n)->next=0))
#define DLLPushBack(f, l, n) DLLPushBackNP(f, l, n, next, prev);
#define DLLPushFrontNP(f, l, n, next, prev) DLLPushBackNP(l, f, n, prev, next)
#define DLLPushFront(f, l, n) DLLPushBackNP(l, f, n, prev, next)
#define DLLRemoveNP(f,l,n,next,prev) ((f)==(l)&&(f)==(n)?\
((f)=(l)=0):\
((f)==(n)?\
((f)=(f)->next,(f)->prev=0):\
((l)==(n)?\
((l)=(l)->prev,(l)->next=0):\
((n)->prev->next=(n)->next,\
(n)->next->prev=(n)->prev))))
#define DLLRemove(f,l,n) DLLRemoveNP(f, l, n, next, prev)


#define QueuePushN(f, l, n, next) ((f)==0?\
((f)=(l)=(n), (n)->next=0):\
((l)->next=(n), (l)=(n)), \
(n)->next=0)
#define QueuePush(f, l, n) QueuePushN(f, l, n, next)
#define QueuePushFrontN(f, l, n, next) ((f)==0?\
((f)=(l)=(n), (n)->next=0):\
((n)->next=(f), (f)=(n)))
#define QueuePushFront(f, l, n) QueuePushFrontN(f, l, n, next)
#define QueuePopN(f, l, next) ((f)==(l)?\
((f)=(l)=0):\
((f)=(f)->next))
#define QueuePop(f, l) QueuePopN(f, l, next)


#define StackPushN(f, n, next) ((n)->next=(f), (f)=(n))
#define StackPush(f, n) StackPushN(f, n, next)
#define StackPopN(f, next) ((f)==0?0:\
((f)=(f)->next))
#define StackPop(f) StackPopN(f, next)


////////////////////////////
// NOTE(nates): Base Types

typedef enum Axis
{
 Axis_X,
 Axis_Y,
 Axis_Z,
 Axis_W,
} Axis;

typedef enum Side
{
 Side_Min,
 Side_Max,
} Side;

typedef enum OperatingSystem
{
 OperatingSystem_Null,
 OperatingSystem_Windows,
 OperatingSystem_Linux,
 OperatingSystem_Mac,
 OperatingSystem_Count,
} OperatingSystem;

typedef enum Architecture
{
 Architecture_Null,
 Architecture_X64,
 Architecture_X86,
 Architecture_Arm,
 Architecture_Arm64,
 Architecture_Count,
} Architecture;

typedef enum Month
{
 Month_Jan,
 Month_Feb,
 Month_Mar,
 Month_Apr,
 Month_May,
 Month_Jun,
 Month_Jul,
 Month_Aug,
 Month_Sep,
 Month_Oct,
 Month_Nov,
 Month_Dec,
} Month;

typedef enum WeekDay
{
 WeekDay_Monday,
 WeekDay_Tuesday,
 WeekDay_Wednsday,
 WeekDay_Thursday,
 WeekDay_Friday,
 WeekDay_Saturday,
 WeekDay_Sunday,
} WeekDay;

///////////////////////////////////
//~ NOTE(nates): Access flags

typedef U32 DataAccessFlags;
enum DataAccessFlags
{
 DataAccessFlag_Read = (1 << 0),
 DataAccessFlag_Write = (1 << 1),
 DataAccessFlag_Execute = (1 << 2),
};

///////////////////////////////
// NOTE(nates): Time

typedef U64 DenseTime;

typedef struct DateTime
{
 U16 ms; // [0,999]
 U8 sec;   // [0,60] -> 60 and not 59 because of leap seconds
 U8 min;   // [0,59]
 U8 hour;  // [0,23]
 U8 day;   // [0,30]
 U8 mon;   // [1,12]
 S16 year; // 1 = 1 CE; 2000 == 2000 CE; 0 = 1 BCE; -100 = 101 BCE
} DateTime;

///////////////////////////////
//~ NOTE(nates): File properties

typedef U32 FilePropertyFlags;
enum FilePropertyFlags
{
 FilePropertyFlag_IsFolder = (1 << 0),
};

typedef struct FileProperties
{
 U64 size;
 FilePropertyFlags flags;
 DenseTime create_time;
 DenseTime modify_time;
 DataAccessFlags access;
} FileProperties;

////////////////////////////////
// NOTE(nates): Base constants

global_ S8 s8_min = (S8)0x80;
global_ S16 s16_min = (S16)0x8000;
global_ S32 s32_min = (S32)0x80000000;
global_ S64 s64_min = (S64)0x8000000000000000llu;

global_ S8 s8_max = (S8)0x7f;
global_ S16 s16_max = (S16)0x7fff;
global_ S32 s32_max = (S32)0x7fffffff;
global_ S64 s64_max = (S64)0x7fffffffffffffffllu;

global_ U8 u8_max = (U8)0xff;
global_ U16 u16_max = (U16)0xffff;
global_ U32 u32_max = (U32)0xffffffff;
global_ U64 u64_max = (U64)0xffffffffffffffffllu;

#include <float.h>
#include <limits.h>
#define R32MAX FLT_MAX
#define S32MAX INT_MAX
#define S32MIN INT_MIN
#define U32MAX UINT_MAX
#define U32MIN 0

#define PI32 3.14159265359f
#define TAU32 6.28318530718f

#define DegreesToRadians (PI32 / 180.0f)
#define RadiansToDegrees (180.0f / PI32)

////////////////////////////////////
// NOTE(nates): World enums functions

func_ OperatingSystem OperatingSystemContext(void);
func_ Architecture ArchitectureContext(void);
func_ char *StringFromOperatingSistem(OperatingSystem OS);
func_ char *StringFromArchitecture(Architecture Arch);
func_ char *StringFromMonth(Month month);
func_ char *StringFromWeekDay(WeekDay day);

//////////////////////////////////////////
// NOTE(nates): Date time && Dense time functions

func_ DenseTime DenseTimeFromDateTime(DateTime *time);
func_ DateTime  DateTimeFromDenseTime(DenseTime time);


#endif //BASE_TYPES_H