/* date = August 24th 2022 5:32 am */
#ifndef CODE_GEN_TOKEN_H
#define CODE_GEN_TOKEN_H

/////////////////////////////////////////
//~ NOTE(nates): Types

typedef U32 Token_Kind;
enum Token_Kind
{
 Token_Kind_ERROR,
 
 Token_Kind_NewLine,
 Token_Kind_Whitespace,
 
 Token_Kind_Keyword,
 Token_Kind_Identifier,
 Token_Kind_TableParam,
 Token_Kind_NumberLiteral,
 
 // nates: all token kinds below are single characters and the tokens share the same
 // code path
 Token_Kind_OpenParen,
 Token_Kind_CloseParen,
 Token_Kind_OpenBrace,
 Token_Kind_CloseBrace,
 Token_Kind_OpenBracket,
 Token_Kind_CloseBracket,
 Token_Kind_LeftArrow,
 Token_Kind_RightArrow,
 
 Token_Kind_Comma,
 Token_Kind_Period,
 Token_Kind_Semicolon,
 Token_Kind_Colon,
 
 Token_Kind_Exclamation,
 // nates: @ is reserved for keywords
 Token_Kind_Pound,
 Token_Kind_Percent,
 Token_Kind_Caret,
 Token_Kind_AndPersand,
 Token_Kind_Asterisk,
 Token_Kind_Minus,
 Token_Kind_Equals,
 Token_Kind_Plus,
 Token_Kind_ForwardSlash,
 Token_Kind_BackSlash,
 Token_Kind_QuestionMark,
 Token_Kind_Vertical,
 
 Token_Kind_SingleQuote,
 Token_Kind_DoubleQuote,
 Token_Kind_Tick,
 Token_Kind_Tilde,
 
 Token_Kind_EOF,
 
 Token_Kind_COUNT,
};

global_ char *token_kind_strings[] = {
 "Token Kind Error",
 
 "NewLine",
 "Whitespace",
 
 "Keyword",
 "Identifier",
 "TableParam",
 "NumberLiteral",
 
 "OpenParen",
 "CloseParen",
 "OpenBrace",
 "CloseBrace",
 "OpenBracket",
 "CloseBracket",
 "LeftArrow",
 "RightArrow",
 
 "Comma",
 "Period",
 "Semicolon",
 "Colon",
 
 "Exclamation",
 // nates: @ is reserved for keywords
 "Pound",
 "Percent",
 "Caret",
 "AndPersand",
 "Asterisk",
 "Minus",
 "Equals",
 "Plus",
 "ForwardSlash",
 "BackSlash",
 "QuestionMark",
 
 "SingleQuote",
 "DoubleQuote",
 "Tick",
 "Tilde",
 
 "End of file",
 
};

typedef struct Token
{
 S64 pos;
 S64 line;
 S64 col;
 S64 size;
 Token_Kind kind;
} Token;

typedef struct Token_Node Token_Node;
struct Token_Node
{
 Token_Node *next;
 Token token;
};

typedef struct Token_Iter
{
 Token_Node *first;
 Token_Node *last;
 U32 token_count;
} Token_Iter;

//////////////////////////
//~ NOTE(nates): Functions

func_ Token_Iter TokenizeData(String8 data);
func_ Token GrabTokenIter(Token_Iter *iter);
func_ Token PeekTokenIter(Token_Iter *iter);
func_ void  OmitTokenIter(Token_Iter *iter);
func_ void  SkipWhitespaceIter(Token_Iter *iter);


#endif //CODE_GEN_TOKEN_H
