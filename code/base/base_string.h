/* date = August 18th 2022 4:04 pm */
#ifndef BASE_STRING_H
#define BASE_STRING_H

////////////////////////////////
//~ NOTE(nates): String8 types

typedef struct String8
{
 U8 *str;
 U64 size;
} String8;

typedef struct String8Node String8Node;
struct String8Node
{
 String8Node *next;
 String8 string;
};

typedef struct String8List
{
 String8Node *first;
 String8Node *last;
 U64 count;
 U64 total_size;
}String8List;

typedef struct StringJoin
{
 String8 pre;
 String8 mid;
 String8 post;
}StringJoin;

typedef U32 StringMatchFlags;
enum
{
 StringMatchFlag_NoCase = (1 << 0),
};

///////////////////////////////////////
//~ NOTE(nates): Unicode string type

typedef struct String16
{
 U16 *str;
 U64 size;
}String16;

typedef struct String32
{
 U32 *str;
 U64 size;
} String32;

typedef struct StringDecode
{
 U32 codepoint;
 U32 size;
} StringDecode;

///////////////////////////////////////////////////
//~ NOTE(nates): Character functions

func_ U8 U8Uppercase(U8 value);
func_ U8 U8Lowercase(U8 value);
func_ B32 CharIsSlash(U8 character);

////////////////////////////////////////////////////
//~ NOTE(nates): Immutable string8 helper functions

func_ String8 Str8(U8 *str, U64 size);
func_ String8 Str8Rng(U8 *first, U8 *opl);
func_ String8 Str8CStr(U8 *cstr);
func_ char   *CStrStr8(Arena *arena, String8 string);
func_ String8 PushCopyStr8(Arena *arena, String8 string);

///////////////////////////////////////////////
//~ NOTE(nates): Immutable string16 helper functions

func_ String16 Str16(U16 *str, U64 size);
func_ String16 Str16CStr(U16 *str);

////////////////////////////////////////////////
//~ NOTE(nates): String8 manipulation functions

#define Str8Lit(s) Str8((U8 *)(s), sizeof(s) - 1)

func_ String8 PrefixStr8(String8 str, U64 size);
func_ String8 ChopStr8(String8 str, U64 amount);
func_ String8 PosfixStr8(String8 str, U64 size);
func_ String8 SkipStr8(String8 str, U64 amount);
func_ String8 SubstrStr8(String8 str, U64 first, U64 opl);
func_ String8 SubstrSizeStr8(String8 str, U64 first, U64 size);

#define Str8Expand(s) (int)((s).size), ((s).str)

func_ String8 ChopAtLastSlashStr8(String8 str);

func_ S32 S32FromStr8(String8 str);
func_ S64 S64FromStr8(String8 str);
func_ U32 U32FromStr8(String8 str);
func_ U64 U64FromStr8(String8 str);

func_ String8 Str8FromU64(Arena *arena, U64 num);

/////////////////////////////////////////////////
//~ NOTE(nates): String helper functions

func_ B32 Str8Match(String8 a, String8 b, StringMatchFlags flags);
func_ String8 FindFirstStr8(String8 haystack, String8 needle, StringMatchFlags flags);

// NOTE(nates): Hashing

func_ U64 HashStr8(String8 string);

/////////////////////////////////////////
//~ NOTE(nates): String list functions

func_ void PushExplicitStr8List(String8List *list, String8 string, String8Node *node);
func_ void PushStr8List(Arena *arena, String8List *list, String8 string);
func_ String8 JoinStr8List(Arena *arena, String8List *list, StringJoin *optional_join);
func_ String8List SplitStr8List(Arena *arena, String8 string, U8 *splits, U32 count);

/////////////////////////////////////
//~ NOTE(nates): Format strings

func_ String8 PushFVStr8(Arena *arena, char *fmt, va_list args);
func_ String8 PushFStr8(Arena *arena, char *fmt, ...);
func_ void ListPushFStr8(Arena *arena, String8List *list, char *fmt, ...);

/////////////////////////////////////////
//~ NOTE(nates): Unicode Helpers

func_ StringDecode DecodeUtf8Str(U8 *str, U64 cap);
func_ U32 EncodeUtf8Str(U8 *dst, U32 codepoint);
func_ StringDecode DecodeUtf16Str(U16 *str, U32 cap);
func_ U32 EncodeUtf16Str(U16 *dst, U32 codepoint);

func_ String32 Str32FromStr8(Arena *arena, String8 string);
func_ String8 Str8FromStr32(Arena *arena, String32 string);
func_ String16 Str16FromStr8(Arena *arena, String8 string);
func_ String8 Str8FromStr16(Arena *arena, String16 string);


#endif //BASE_STRING_H
