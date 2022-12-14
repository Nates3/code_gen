
global_ U8 char_pred_whitespace[32] = {
 0x0, 0x2, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

global_ U8 char_pred_number[32] = {
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0x3,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

global_ U8 char_pred_alpha_numeric_underscore[32] = {
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0x3,
 0xFE, 0xFF, 0xFF, 0x87, 0xFE, 0xFF, 0xFF, 0x7,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

global_ U8 char_pred_alpha[32] = {
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
 0xFE, 0xFF, 0xFF, 0x7, 0xFE, 0xFF, 0xFF, 0x7,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};
#define CharacterPredicate(c, p) ((p[(c / 8)]&(1<<(c%8))) != 0)

Arena *token_arena = 0;

func_ Token_Iter 
TokenizeData(String8 input)
{
 Token_Iter iter = {0};
 
 S64 pos = 0;
 S64 line = 1;
 S64 line_pos = -1;
 for(; pos < input.size;
     ) {
  U8 first = input.str[pos];
  switch(first)
  {
   case'\n': {
    Token_Node *node = PushArray(token_arena, Token_Node, 1);
    QueuePush(iter.first, iter.last, node);
    node->token.kind = Token_Kind_NewLine;
    node->token.pos = pos;
    node->token.line = line;
    node->token.col = pos - line_pos;
    node->token.size = 1;
    
    line++;
    line_pos = pos;
    pos++;
   } break;
   
   case' ': case'\t': {
    Token_Node *node = PushArray(token_arena, Token_Node, 1);
    QueuePush(iter.first, iter.last, node);
    node->token.kind = Token_Kind_Whitespace;
    node->token.pos = pos;
    node->token.line = line;
    node->token.col = pos - line_pos;
    for(;CharacterPredicate(input.str[pos], char_pred_whitespace) && pos < input.size; ++pos);
    node->token.size = pos - node->token.pos;
   } break;
   
   case'@': {
    Token_Node *node = PushArray(token_arena, Token_Node, 1);
    QueuePush(iter.first, iter.last, node);
    node->token.kind = Token_Kind_Keyword;
    node->token.pos = pos;
    node->token.line = line;
    node->token.col = pos - line_pos;
    pos++;
    for(;CharacterPredicate(input.str[pos], char_pred_alpha_numeric_underscore) && pos < input.size; ++pos);
    node->token.size = pos - node->token.pos;
    
    B32 valid_keyword = false;
    String8 keyword = SubSizeStr8(input, node->token.pos, node->token.size);
    for(U32 index = 0; index < ArrayCount(language_keywords); ++index) {
     String8 language_keyword = Str8CStr((U8 *)language_keywords[index]);
     if(Str8Match(language_keyword, keyword, 0)) {
      valid_keyword = true;
     }
    }
    if(valid_keyword == false) {
     ArenaTemp scratch = GetScratch(0, 0);
     String8 keyword_string = SubSizeStr8(input, node->token.pos, node->token.size);
     String8 keyword_cstr = CopyStr8(scratch.arena, keyword_string);
     fprintf(stderr, "Error: invalid keyword(%s) [%llu:%llu]\n", 
             keyword_cstr.str,
             node->token.line, node->token.col);
     ReleaseScratch(scratch);
     OS_Abort();
    }
   } break;
   
   case'a': case'b': case'c': case'd': case'e': case'f': case'g': case'h': case'i': 
   case'j': case'k': case'l': case'm': case'n': case'o': case'p': case'q': case'r': 
   case's': case't': case'u': case'v': case'w': case'x': case'y': case'z': case'A': 
   case'B': case'C': case'D': case'E': case'F': case'G': case'H': case'I': case'J':
   case'K': case'L': case'M': case'N': case'O': case'P': case'Q': case'R': case'S': 
   case'T': case'U': case'V': case'W': case'X': case'Y': case'Z': case'_': {
    Token_Node *node = PushArray(token_arena, Token_Node, 1);
    QueuePush(iter.first, iter.last, node);
    node->token.kind = Token_Kind_Identifier;
    node->token.pos = pos;
    node->token.line = line;
    node->token.col = pos - line_pos;
    for(;CharacterPredicate(input.str[pos], char_pred_alpha_numeric_underscore) && pos < input.size; ++pos);
    node->token.size = pos - node->token.pos;
   } break;
   
   case'$': {
    Token_Node *node = PushArray(token_arena, Token_Node, 1);
    QueuePush(iter.first, iter.last, node);
    node->token.kind = Token_Kind_TableParam;
    node->token.pos = pos;
    node->token.line = line;
    node->token.col = pos - line_pos;
    pos++;
    if(!CharacterPredicate(input.str[pos], char_pred_number)) {
     for(;CharacterPredicate(input.str[pos], char_pred_alpha_numeric_underscore) && pos < input.size; ++pos);
     node->token.size = pos - node->token.pos;
    }
    else {
     fprintf(stderr, "Error: table parameter can't start as a numeric [%llu:%llu]\n", line, pos - line_pos);
     OS_Abort();
    }
   } break;
   
   case '0': case'1': case'2': case'3': case'4': case'5': case'6': case'7': case'8': case'9': {
    Token_Node *node = PushArray(token_arena, Token_Node, 1);
    QueuePush(iter.first, iter.last, node);
    node->token.kind = Token_Kind_NumberLiteral;
    node->token.pos = pos;
    node->token.line = line;
    node->token.col = pos - line_pos;
    for(;CharacterPredicate(input.str[pos], char_pred_number) && pos < input.size; ++pos);
    node->token.size = pos - node->token.pos;
   } break;
   
   // nates: single character tokens with same path
   Token_Kind single_kind = Token_Kind_ERROR;
   case'(': { single_kind = Token_Kind_OpenParen; } goto Shared_Path;
   case')': { single_kind = Token_Kind_CloseParen; } goto Shared_Path;
   case'{': { single_kind = Token_Kind_OpenBrace; } goto Shared_Path;
   case'}': { single_kind = Token_Kind_CloseBrace; } goto Shared_Path;
   case'[': { single_kind = Token_Kind_OpenBracket; } goto Shared_Path;
   case']': { single_kind = Token_Kind_CloseBracket; } goto Shared_Path;
   case'<': { single_kind = Token_Kind_LeftArrow; } goto Shared_Path;
   case'>': { single_kind = Token_Kind_RightArrow; } goto Shared_Path;
   case',': { single_kind = Token_Kind_Comma; } goto Shared_Path;
   case'.': { single_kind = Token_Kind_Period; } goto Shared_Path;
   case';': { single_kind = Token_Kind_Semicolon; } goto Shared_Path;
   case':': { single_kind = Token_Kind_Colon; } goto Shared_Path;
   case'!': { single_kind = Token_Kind_Exclamation; } goto Shared_Path;
   case'#': { single_kind = Token_Kind_Pound; } goto Shared_Path;
   case'%': { single_kind = Token_Kind_Percent; } goto Shared_Path;
   case'^': { single_kind = Token_Kind_Caret; } goto Shared_Path;
   case'&': { single_kind = Token_Kind_AndPersand; } goto Shared_Path;
   case'*': { single_kind = Token_Kind_Asterisk; } goto Shared_Path;
   case'-': { single_kind = Token_Kind_Minus; } goto Shared_Path;
   case'=': { single_kind = Token_Kind_Equals; } goto Shared_Path;
   case'+': { single_kind = Token_Kind_Plus; } goto Shared_Path;
   case'/': { single_kind = Token_Kind_ForwardSlash; } goto Shared_Path;
   case'\\': { single_kind = Token_Kind_BackSlash; } goto Shared_Path;
   case'?': { single_kind = Token_Kind_QuestionMark; } goto Shared_Path;
   case'|': { single_kind = Token_Kind_Vertical; } goto Shared_Path;
   case'\'': { single_kind = Token_Kind_SingleQuote; } goto Shared_Path;
   case'"': { single_kind = Token_Kind_DoubleQuote; } goto Shared_Path;
   case'`': { single_kind = Token_Kind_Tick; } goto Shared_Path;
   case'~': { single_kind = Token_Kind_Tilde; } goto Shared_Path;
   case 0: { fprintf(stderr, "Error: unexpected eof token parse\n"); OS_Abort();}
   
   {
    Shared_Path:;
    if(single_kind != Token_Kind_ERROR) {
     Token_Node *node = PushArray(token_arena, Token_Node, 1);
     QueuePush(iter.first, iter.last, node);
     node->token.kind = single_kind;
     node->token.pos = pos;
     node->token.line = line;
     node->token.col = pos - line_pos;
     node->token.size = 1;
     
     pos++;
    }
    else {
     fprintf(stderr, "Error: single_kind character %c failed to set a valid token kind", first);
     OS_Abort();
    }
   } break;
   
   default: {
    fprintf(stderr, "Error: unhandled token case\n");
    OS_Abort();
   } break;
  }
  iter.token_count++;
 }
 
 Token_Node *eof = PushArray(token_arena, Token_Node, 1);
 QueuePush(iter.first, iter.last, eof);
 eof->token.kind = Token_Kind_EOF;
 eof->token.pos = 0;
 eof->token.line = 0;
 eof->token.col = 0;
 eof->token.size = 1;
 iter.token_count++;
 
 return(iter);
}

func_ Token 
GrabTokenIter(Token_Iter *iter)
{
 Token result = iter->first->token;
 QueuePop(iter->first, iter->last);
 return(result);
}

func_ Token 
PeekTokenIter(Token_Iter *iter)
{
 Token result = iter->first->token;
 return(result);
}

func_ void 
OmitTokenIter(Token_Iter *iter)
{
 QueuePop(iter->first, iter->last);
}

func_ void 
SkipWhitespaceIter(Token_Iter *iter)
{
 for(;;) {
  Token token = PeekTokenIter(iter);
  if((token.kind != Token_Kind_Whitespace && token.kind != Token_Kind_NewLine)) {
   break;
  }
  
  OmitTokenIter(iter);
 }
}