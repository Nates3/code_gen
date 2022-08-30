
func_ U8 
U8Uppercase(U8 value)
{
 if('a' < value && value < 'z') {
  value -= 0x20;
 }
 return(value);
}

func_ U8 
U8Lowercase(U8 value)
{
 if('A' < value && value < 'Z') {
  value += 0x20;
 }
 return(value);
}

func_ B32
CharIsSlash(U8 character)
{
 B32 result = ((character == '\\') || (character == '/'));
 return(result);
}

func_ String8
Str8(U8 *str, U64 size)
{
 String8 result = {str, size};
 return(result);
}

func_ String8 
Str8Rng(U8 *first, U8 *opl)
{
 U64 string_size = (U64)opl - (U64)first;
 String8 result = Str8(first, string_size);
 return(result);
}

func_ String8 
Str8CStr(U8 *cstr)
{
 U8 *search = cstr;
 for(;;) {
  if(*search == 0) {
   break;
  }
  search += 1;
 }
 
 U64 size = (U64)search - (U64)cstr;
 String8 result = {cstr, size};
 return(result);
}

func_ char *
CStrStr8(Arena *arena, String8 string)
{
 U8 *memory = PushArray(arena, U8, string.size + 1);
 U8 *ptr = memory;
 for(U32 index = 0;
     index < string.size;
     ++index) {
  *ptr = string.str[index];
  ptr += 1;
 }
 
 *ptr = 0;
 return((char *)memory);
}

func_ String8
PushCopyStr8(Arena *arena, String8 string)
{
 U8 *memory = PushArray(arena, U8, string.size + 1);
 MemoryCopy(memory, string.str, string.size);
 memory[string.size] = 0;
 String8 result = {
  .str = memory,
  .size = string.size,
 };
 return(result);
}



/////////////////////////
// NOTE(nates): String16 functions

func_ String16
Str16(U16 *str, U64 size)
{
 String16 result = {str, size};
 return(result);
}

func_ String16 
Str16CStr(U16 *str)
{
 String16 result = {0};
 result.str = str;
 U16 *ptr = str;
 for(;*ptr != 0; ptr += 1);
 result.size = ptr - str;
 return(result);
}

//////////////////////////////////////////////////
// NOTE(nates): String8 manipulation functions

func_ String8
PrefixStr8(String8 str, U64 size)
{
 U64 size_clamped = ClampTop(size, str.size);
 String8 result = {str.str, size_clamped};
 return(result);
}

func_ String8
ChopStr8(String8 str, U64 amount)
{
 U64 amount_clamped = ClampTop(amount, str.size);
 U64 remaining_size = str.size - amount_clamped;
 String8 result = {str.str, remaining_size};
 return(result);
}

func_ String8
PostfixStr8(String8 str, U64 size)
{
 U64 size_clamped = ClampTop(size, str.size);
 U64 skip_to = str.size - size_clamped;
 String8 result = {str.str + skip_to, size_clamped};
 return(result);
}

func_ String8
SkipStr8(String8 str, U64 amount)
{
 U64 skip_clamped = ClampTop(amount, str.size);
 U64 remaining_size = str.size - skip_clamped;
 String8 result = {str.str + skip_clamped, remaining_size };
 return(result);
}

func_ String8 
SubstrStr8(String8 str, U64 first, U64 opl)
{
 U64 clamped_end = ClampTop(opl, str.size);
 U64 string_size = clamped_end - first;
 String8 result = {str.str + first, string_size};
 return(result);
}

func_ String8
SubstrSizeStr8(String8 str, U64 first, U64 size)
{
 String8 result = SubstrStr8(str, first, first + size);
 return(result);
}

func_ String8 
ChopAtLastSlashStr8(String8 str)
{
 String8 result = str;
 if(str.size)
 {
  U64 last_slash = 0;
  U8 *opl = str.str + str.size;
  for(U8 *ptr = str.str;ptr < opl;ptr += 1) {
   if(CharIsSlash(*ptr)) {
    last_slash = (U64)(ptr - str.str);
   }
  }
  
  result = PrefixStr8(str, last_slash + 1);
 }
 return(result);
}

func_ S32 
S32FromStr8(String8 str)
{
 String8 num_part = str;
 B32 is_negative = false;
 if(str.str[0] == '-')
 {
  num_part = SkipStr8(str, 1);
  is_negative = true;
 }
 
 U32 result = 0;
 U64 exponent = num_part.size;
 for(U8 *ptr = num_part.str, *opl = num_part.str + num_part.size; ptr < opl; ptr += 1) {
  U32 digit = (U32)(*ptr) - 0x30;
  if(0 > digit || digit > 9) {
   InvalidPath;
  }
  for(U64 i = 1; i < exponent; ++i) {
   digit *= 10;
  }
  result += digit;
  exponent--;
 }
 
 if(is_negative)
 {
  result *= -1;
 }
 
 return(result);
}

func_ S64 
S64FromStr8(String8 str)
{
 String8 num_part = str;
 B32 is_negative = false;
 if(str.str[0] = '-')
 {
  num_part = SkipStr8(str, 1);
  is_negative = true;
 }
 
 U64 result = 0;
 U64 exponent = num_part.size;
 for(U8 *ptr = num_part.str, *opl = num_part.str + num_part.size; ptr < opl; ptr += 1) {
  U64 digit = (U64)(*ptr) - 0x30;
  if(0 > digit || digit > 9) {
   InvalidPath;
  }
  for(U64 i = 1; i < exponent; ++i) {
   digit *= 10;
  }
  result += digit;
  exponent--;
 }
 
 if(is_negative)
 {
  result *= -1;
 }
 
 return(result);
}

func_ U32 
U32FromStr8(String8 str)
{
 U32 result = 0;
 U64 exponent = str.size;
 for(U8 *ptr = str.str, *opl = str.str + str.size; ptr < opl; ptr += 1) {
  U32 digit = (U32)(*ptr) - 0x30;
  if(0 > digit || digit > 9) {
   InvalidPath;
  }
  for(U64 i = 1; i < exponent; ++i) {
   digit *= 10;
  }
  result += digit;
  exponent--;
 }
 return(result);
}
func_ U64 
U64FromStr8(String8 str)
{
 U64 result = 0;
 U64 exponent = str.size;
 for(U8 *ptr = str.str, *opl = str.str + str.size; ptr < opl; ptr += 1) {
  U64 digit = (U64)(*ptr) - 0x30;
  if(0 > digit || digit > 9) {
   InvalidPath;
  }
  for(U64 i = 1; i < exponent; ++i) {
   digit *= 10;
  }
  result += digit;
  exponent--;
 }
 return(result);
}

func_ String8
Str8FromU64(Arena *arena, U64 num)
{
 ArenaTemp scratch = GetScratch(arena, 0);
 String8 result = {0};
 String8List digits = {0};
 for(;;) {
  U64 bottom_digit = num % 10;
  num /= 10;
  String8Node *node = PushArray(scratch.arena, String8Node, 1);
  node->string.str = PushArray(scratch.arena, U8, 1);
  node->string.size = 1;
  node->string.str[0] = bottom_digit + 0x30;
  QueuePushFront(digits.first, digits.last, node);
  digits.count += 1;
  digits.total_size += node->string.size;
  if(num == 0) { break; }
 }
 
 result = JoinStr8List(arena, &digits, 0);
 return(result);
}

func_ B32 
Str8Match(String8 a, String8 b, StringMatchFlags flags)
{
 B32 result = false;
 if(a.size == b.size) {
  result = true;
  for(U32 index = 0; index < a.size; ++index) {
   B32 no_case = HasFlags(flags, StringMatchFlag_NoCase);
   U8 ca = a.str[index];
   U8 cb = b.str[index];
   if(no_case) {
    ca = U8Uppercase(ca);
    cb = U8Uppercase(cb);
   }
   if(ca != cb) {
    result = false;
    break;
   }
  }
 }
 return(result);
}

func_ String8 
FindFirstStr8(String8 haystack, String8 needle, StringMatchFlags flags)
{
 String8 result = {0};
 if(haystack.size >= needle.size) {
  U64 constrained_size = haystack.size - needle.size;
  for(U64 pos = 0; pos <= constrained_size; ++pos) {
   String8 test = Str8(haystack.str + pos, needle.size);
   if(Str8Match(test, needle, flags)) {
    result = test;
    break;
   }
  }
 }
 
 
 return(result);
}

func_ U64
HashStr8(String8 string)
{
 U64 hash = 0;
 for(U8 *p = string.str, *opl = string.str + string.size;
     p < opl; p += 1) {
  U8 v = *p;
  hash = ((hash << 5) + hash) + v;
 }
 return(hash);
}

func_ void 
PushExplicitStr8List(String8List *list, String8 string, 
                     String8Node *node)
{
 node->string = string;
 QueuePush(list->first, list->last, node);
 list->count += 1;
 list->total_size += string.size;
}

func_ void 
PushStr8List(Arena *arena, String8List *list, String8 string)
{
 String8Node *new_node = PushArray(arena, String8Node, 1);
 PushExplicitStr8List(list, string, new_node);
}

func_ String8 
JoinStr8List(Arena *arena, String8List *list, StringJoin *optional_join)
{
 local_ StringJoin dummy_join = {0};
 StringJoin *join = optional_join;
 if(join == 0) {
  join = &dummy_join;
 }
 U64 size = (join->pre.size + 
             join->post.size +
             join->mid.size*(list->count - 1) +
             list->total_size);
 
 U8 *str = PushArray(arena, U8, size + 1);
 U8 *ptr = str;
 
 MemoryCopy(ptr, join->pre.str, join->pre.size);
 ptr += join->pre.size;
 
 B32 is_mid = false;
 
 U64 pos = 0;
 for(String8Node *node = list->first;
     node != 0;
     node = node->next) {
  if(is_mid) {
   MemoryCopy(ptr, join->mid.str, join->mid.size);
   ptr += join->mid.size;
  }
  
  MemoryCopy(ptr, node->string.str, node->string.size);
  ptr += node->string.size;
  
  is_mid = (pos == list->count/2);
  pos += 1;
 }
 
 MemoryCopy(ptr, join->post.str, join->post.size);
 ptr += join->post.size;
 
 MemoryZero(ptr, 1);
 
 String8 result = {
  .str = str,
  .size = size,
 };
 
 return(result);
}

func_ String8List 
SplitStr8List(Arena *arena, String8 string, U8 *splits, U32 count)
{
 String8List result = {0};
 
 U8 *ptr = string.str;
 U8 *word_first = ptr;
 U8 *opl = string.str + string.size;
 
 for(;ptr < opl; ptr += 1) {
  U8 byte = *ptr;
  B32 is_split_byte = false;
  for(U32 i = 0; i < count; ++i) {
   if(byte == splits[i]) {
    is_split_byte = true;
    break;
   }
  }
  
  if(is_split_byte) {
   if(word_first < ptr) {
    Str8ListPush(arena, &result, Str8Rng(word_first, ptr));
   }
   word_first = ptr + 1;
  }
 }
 
 if(word_first < ptr) {
  Str8ListPush(arena, &result, Str8Rng(word_first, ptr));
 }
 return(result);
}

func_ String8
PushFVStr8(Arena *arena, char *fmt, va_list args)
{
 va_list args2;
 va_copy(args2, args);
 
 U64 buffer_size = 1024;
 U8 *buffer = PushArray(arena, U8, buffer_size);
 U64 actual_size = vsnprintf((char *)buffer, buffer_size, fmt, args);
 
 String8 result = {0};
 if(actual_size < buffer_size) {
  U64 size_that_we_didnt_use = buffer_size - actual_size;
  ArenaPop(arena, size_that_we_didnt_use);
 }
 else {
  ArenaPop(arena, buffer_size);
  U8 *fixed_buffer = PushArray(arena, U8, actual_size + 1);
  U64 final_size = vsnprintf((char *)fixed_buffer, actual_size + 1, fmt, args2);
  result = Str8(fixed_buffer, final_size);
 }
 va_end(args2);
 return(result);
}

func_ String8
PushFStr8(Arena *arena, char *fmt, ...)
{
 va_list args;
 va_start(args, fmt);
 String8 result = PushFVStr8(arena, fmt, args);
 va_end(args);
 return(result);
}

func_ void
PushFStr8List(Arena *arena, String8List *list, char *fmt, ...)
{
 va_list args;
 va_start(args, fmt);
 String8 string = PushFVStr8(arena, fmt, args);
 va_end(args);
 Str8ListPush(arena, list, string);
}

//~ NOTE(nates): Unicode helpers implementation

func_ StringDecode 
DecodeUtf8Str(U8 *str, U64 cap)
{
 local_ U8 lengths[] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 
  2, 2, 2, 2, 
  3, 3, 
  4,
  0,
 };
 
 local_ U8 first_byte_masks[5] = {0x0, 0x7f, 0x1f, 0x0f, 0x07};
 local_ U8 final_shifts[5] = {18, 12, 6, 0};
 
 StringDecode result = {
  .codepoint = '#',
  .size = 1,
 };
 
 if(cap > 0) {
  U8 len = lengths[str[0] >> 3];
  U32 codepoint = 0;
  if(0 < len && len <= cap) {
   codepoint |= (str[0]&first_byte_masks[len]) << 18;
   result.size = len;
   len--;
   switch(len) {
    case 3: codepoint |= (str[3]&0x3f) << 0;
    case 2: codepoint |= (str[2]&0x3f) << 6;
    case 1: codepoint |= (str[1]&0x3f) << 12;
    case 0: codepoint >>= final_shifts[len];
   }
   result.codepoint = codepoint;
  }
 }
 return(result);
}

func_ U32 
EncodeUtf8Str(U8 *dst, U32 codepoint)
{
 U32 size = 0;
 if(codepoint < (1 << 8)) {
  dst[0] = codepoint;
  size = 1;
 }
 else if(codepoint < (1 << 11)) {
  dst[0] = 0xc0 | (codepoint >> 6);
  dst[1] = 0x80 | (codepoint&0x3f);
  size = 2;
 }
 else if(codepoint < (1 << 16)) {
  dst[0] = 0xe0 | (codepoint >> 12);
  dst[1] = 0x80 | ((codepoint >> 6)&0x3f);
  dst[2] = 0x80 | (codepoint&0x3f);
  size = 3;
 }
 else if(codepoint < (1 << 21)) {
  dst[0] = 0xf0 | (codepoint >> 18);
  dst[1] = 0x80 | ((codepoint >> 12)&0x3f);
  dst[2] = 0x80 | ((codepoint >> 6)&0x3f);
  dst[3] = 0x80 | (codepoint&0x3f);
  size = 4;
 }
 else {
  dst[0] = '#';
  size = 1;
 }
 
 return(size);
}

func_ StringDecode 
DecodeUtf16Str(U16 *str, U32 cap)
{
 StringDecode result = {'#', 1};
 U16 x = str[0];
 if(0xD800 < x && x < 0xDFFF) {
  if(cap >= 2) {
   U16 y = str[1];
   if((0xD800 <= x && x < 0xDC00) &&
      (0xDC00 <= y && y < 0xDE00)) {
    U16 xj = x - 0xD800;
    U16 yj = y - 0xDC00;
    
    result.codepoint = ((y << 10) | (x)) + 0x10000;
    result.size = 2;
   }
  }
 }
 else {
  result.codepoint = str[0];
  result.size = 1;
 }
 
 return(result);
}

func_ U32 
EncodeUtf16Str(U16 *dst, U32 codepoint)
{
 U32 size = 0;
 if(codepoint < 0x10000) {
  dst[0] = codepoint;
  size = 1;
 }
 else {
  U32 cpj = codepoint - 0x10000;
  dst[0] = (cpj >> 10) + 0xD800;
  dst[1] = (cpj&0x3FF) + 0xDC00;
  size = 2;
 }
 return(size);
}

func_ String32 
Str32FromStr8(Arena *arena, String8 string)
{
 U64 alloc_count = string.size + 1;
 U32 *memory = PushArray(arena, U32, alloc_count);
 
 U32 *dptr = memory;
 U8 *ptr = string.str;
 U8 *opl = string.str + string.size;
 for(;;) {
  StringDecode decode = DecodeUtf8Str(ptr, (U64)(opl - ptr));
  *dptr = decode.codepoint;
  ptr += decode.size;
  dptr += 1;
  
  if(ptr == opl) {
   break;
  }
 }
 
 *dptr = 0;
 
 U64 string_count = (U64)(dptr - memory);
 U64 unused_count = alloc_count - string_count - 1;
 ArenaPop(arena, unused_count*sizeof(*memory));
 
 String32 result = {0};
 result.str = memory;
 result.size = string_count;
 return(result);
}

func_ String8
Str8FromStr32(Arena *arena, String32 string)
{
 U64 alloc_count = string.size*4 + 1;
 U8 *memory = PushArray(arena, U8, alloc_count);
 
 U8 *dptr = memory;
 U32 *ptr = string.str;
 U32 *opl = string.str + string.size;
 for(;;) {
  U64 size = StrEncodeUtf8(dptr, *ptr);
  ptr += 1;
  dptr += size;
  
  if(ptr == opl) {
   break;
  }
 }
 
 *dptr = 0;
 
 U64 string_count = (U64)(dptr - memory);
 U64 unused_count = alloc_count - string_count - 1;
 ArenaPop(arena, unused_count);
 
 String8 result = {0};
 result.str = memory;
 result.size = string_count;
 
 return(result);
}



func_ String16
Str16FromStr8(Arena *arena, String8 string)
{
 U64 alloc_count = string.size*2 + 1;
 U16 *memory = PushArray(arena, U16, alloc_count);
 
 U16 *dptr = memory;
 U8 *ptr = string.str;
 U8 *opl = string.str + string.size;
 for(;;) {
  StringDecode decode = DecodeUtf8Str(ptr, (U64)(opl - ptr));
  U32 enc_size = EncodeUtf16Str(dptr, decode.codepoint);
  ptr += decode.size;
  dptr += enc_size;
  
  if(ptr == opl) {
   break;
  }
 }
 
 *dptr = 0;
 
 U64 string_count = (U64)(dptr - memory);
 U64 unused_count = alloc_count - string_count - 1;
 ArenaPop(arena, unused_count*sizeof(*memory));
 
 String16 result = {0};
 result.str = memory;
 result.size = string_count;
 return(result);
}

func_ String8
Str8FromStr16(Arena *arena, String16 string)
{
 U64 alloc_count = string.size*3 + 1;
 U8 *memory = PushArray(arena, U8, alloc_count);
 
 U8 *dptr = memory;
 U16 *ptr = string.str;
 U16 *opl = string.str + string.size;
 for(;;) {
  StringDecode decode = DecodeUtf16Str(ptr, (U64)(opl - ptr));
  U64 enc_size = StrEncodeUtf8(dptr, decode.codepoint);
  ptr += decode.size;
  dptr += enc_size;
  
  if(ptr == opl) {
   break;
  }
 }
 
 *dptr = 0;
 
 U64 string_count = (U64)(dptr - memory);
 U64 unused_count = alloc_count - string_count - 1;
 ArenaPop(arena, unused_count);
 
 String8 result = {0};
 result.str = memory;
 result.size = string_count;
 
 return(result);
}
