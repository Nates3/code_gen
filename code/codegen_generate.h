/* date = August 29th 2022 1:51 pm */
#ifndef CODEGEN_GENERATE_H
#define CODEGEN_GENERATE_H

func_ void Generate_TokenIter(AST_Node_Table *table, String8List *list, AST_Node_TokenIter *iter_node, String8 parse_data, S32 row_index, U32 *gen_index);
func_ void Generate_GeneratorLoop(AST_Node_Table *table, String8List *list, AST_Node_GeneratorLoop *loop, String8 parse_data, U32 *gen_index);
func_ void Generate_Generator(String8List *list, AST_Node_Generator *gen, String8 parse_data);
func_ String8 Generate_AST(AST ast, String8 parse_data);

#endif //CODEGEN_GENERATE_H
