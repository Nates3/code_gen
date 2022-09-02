/* date = August 29th 2022 1:51 pm */
#ifndef CODEGEN_GENERATE_H
#define CODEGEN_GENERATE_H

func_ void Generate_TableParam(String8List *output, AST_Node_Table *table, Token_Iter *iter, String8 input, S32 row_index, Token table_param);
func_ void Generate_Loop(String8List *output, AST_Node_Table *table, Token_Iter *iter, String8 input, S64 *gen_index);
func_ void Generate_Block(String8List *output, AST_Node_GenerateBlock *gen, String8 input);
func_ String8 Generate_AST(AST ast, String8 input);

typedef struct Token_Iter_Node Token_Iter_Node;
struct Token_Iter_Node
{
 Token_Iter_Node *next;
 Token_Iter iter;
};

#endif //CODEGEN_GENERATE_H
