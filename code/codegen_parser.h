/* date = August 24th 2022 5:43 pm */
#ifndef CODEGEN_PARSER_H
#define CODEGEN_PARSER_H

////////////////////////////////
// NOTE(nates): Types

typedef struct AST_Node AST_Node;
typedef String8Node AST_Node_String;

typedef U32 AST_Node_Kind;
enum
{
 AST_Node_Null,
 AST_Node_Kind_Table,
 AST_Node_Kind_GenerateBlock,
};

typedef struct Table_Element Table_Element;
struct Table_Element
{
 Table_Element *next;
 String8List list;
};

typedef struct AST_Node_Table
{
 String8 name;
 
 String8List parameters;
 
 // nates: these are row elements
 U32 element_count;
 Table_Element *first;
 Table_Element *last;
}AST_Node_Table;

// nates: generators just contain tokens and it's up to the generator to generate
// just these tokens by themselves or generate different output based on keywords
typedef struct AST_Node_GenerateBlock
{
 Token table_name;
 // nates: token iterator that points at the very first token to start
 // generating from
 Token_Iter start;
}AST_Node_GenerateBlock;

struct AST_Node
{
 AST_Node *next;
 AST_Node_Kind kind;
 void *data;
};

typedef struct AST AST;
struct AST
{
 // TODO(nates): Probably don't need count
 U32 count;
 AST_Node *first;
 AST_Node *last;
};

typedef struct AST_TableHashNode AST_TableHashNode;
struct AST_TableHashNode
{
 AST_TableHashNode *hash_next;
 String8 table_name;
 AST_Node_Table *table;
};

typedef struct AST_State AST_State;
struct AST_State
{
 AST_TableHashNode *table_nodes[256];
 AST_TableHashNode *free_nodes;
};

////////////////////////////////
// nates: Functions

func_ void      Parse_TableElements(Token_Iter *iter, String8 data, AST_Node_Table *table, B32 brace_required);
func_ void      Parse_Table(AST *ast, Token_Iter *iter, String8 data);
func_ void      Parse_GenerateBlock(AST *ast, Token_Iter *iter, String8 data);
func_ AST       Parse_Tokens(Token_Iter *iter, String8 data);

#endif //CODEGEN_PARSER_H
