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
 AST_Node_Kind_Generator,
 AST_Node_Kind_GeneratorLoop,
 AST_Node_Kind_TokenIter,
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
 
 U32 element_count;
 Table_Element *first;
 Table_Element *last;
}AST_Node_Table;

typedef struct TokenNode TokenNode;
struct TokenNode
{
 TokenNode *next;
 Token token;
};

typedef struct AST_Node_TokenIter
{
 Token_Iter iter;
}AST_Node_TokenIter;

// nates: generator loops contain generator loops or string lists
typedef struct AST_Node_GeneratorLoop
{
 AST_Node *first;
 AST_Node *last;
}AST_Node_GeneratorLoop;

// nates: generators can contain either generator loops or string list nodes
typedef struct AST_Node_GenerateBlock
{
 Token table;
 Token_Iter prelude_iter;
 
 AST_Node *first;
 AST_Node *last;
}AST_Node_Generator;

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

typedef enum Parse_Kind
{
 Parse_Kind_Null,
 Parse_Kind_Elements,
 Parse_Kind_Loop,
} Parse_Kind;

////////////////////////////////
// nates: Functions

func_ void      Parse_TableElements(Token_Iter *iter, String8 data, AST_Node_Table *table, B32 brace_required);
func_ void      Parse_Table(AST *ast, Token_Iter *iter, String8 data);
func_ void      Parse_GeneratorElement(AST_Node *node, Token_Iter *iter, String8 data);
func_ void      Parse_GeneratorLoop(AST_Node *node, Token_Iter *iter, String8 data);
func_ void      Parse_Generator(AST *ast, Token_Iter *iter, String8 data);
func_ AST       Parse_Tokens(Token_Iter *iter, String8 data);

#endif //CODEGEN_PARSER_H
