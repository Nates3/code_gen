

#include "base\base_inc.h"
#include "os\os_inc.h"

#include "base\base_inc.c"
#include "os\os_inc.c"

#include "codegen_language.h"

#include "codegen_token.h"
#include "codegen_token.c"

#include "codegen_parser.h"
#include "codegen_parser.c"

#include "codegen_generate.h"
#include "codegen_generate.c"


func_ void 
PrintASTNodes(AST_Node *node, U32 level)
{
#define RESET_LIST ReleaseScratch(scratch); str_list = (String8List){0}
 ArenaTemp scratch = GetScratch(0, 0);
 
 String8List str_list = {0};
 for(;node != 0; node = node->next) {
  for(U32 i = 0; i < level; ++i) {
   PushStr8List(scratch.arena, &str_list, Str8Lit("- "));
  }
  switch(node->kind) {
   case AST_Node_Kind_Table: {
    fprintf(stdout, "Node: Table\n");
   }break;
   case AST_Node_Kind_Generator: {
    AST_Node_Generator *gen = (AST_Node_Generator *)node->data;
    PushStr8List(scratch.arena, &str_list, Str8Lit("Node: Generator\n"));
    String8 joined = JoinStr8List(scratch.arena, &str_list, 0);
    fprintf(stdout, joined.str);
    RESET_LIST;
    PrintASTNodes(gen->first, level + 1);
   } break;;
   case AST_Node_Kind_GeneratorLoop: {
    AST_Node_GeneratorLoop *loop = (AST_Node_GeneratorLoop *)node->data;
    PushStr8List(scratch.arena, &str_list, Str8Lit("Node: GeneratorLoop\n"));
    String8 joined = JoinStr8List(scratch.arena, &str_list, 0);
    fprintf(stdout, joined.str);
    RESET_LIST;
    PrintASTNodes(loop->first, level + 1);
   } break;
   case AST_Node_Kind_TokenIter: {
    PushStr8List(scratch.arena, &str_list, Str8Lit("Node: TokenIter\n"));
    String8 joined = JoinStr8List(scratch.arena, &str_list, 0);
    fprintf(stdout, joined.str);
    RESET_LIST;
   } break;
   
   case AST_Node_Null: {
    InvalidPath;
   } break;
  }
 }
 
 ReleaseScratch(scratch);
}

int main(int program_argc, char **program_args)
{
 OS_ThreadContext tctx = {0};
 OS_Main_Init(&tctx, program_argc, program_args);
 
 String8List args = OS_GetCommandLineArgs();
 if(args.count == 3) {
  ArenaTemp scratch = GetScratch(0, 0);
  // TODO(nts): have multiple files
  String8 parse_file = args.first->next->string;
  String8 parse_data = OS_ReadFile(scratch.arena, parse_file);
  
  // nates: create token_arena and parse the file data into tokens
  token_arena = ArenaAlloc(GB(16));
  Token_Iter iter = TokenizeData(parse_data);
  
  // nates:  create arena for the AST node arena using the tokens
  ast_arena = ArenaAlloc(GB(16));
  AST ast = Parse_Tokens(&iter, parse_data);
  
  // nates: generate code using the AST
  gen_arena = ArenaAlloc(GB(16));
  String8 entire_data = Generate_AST(ast, parse_data);
  String8 output_file = args.first->next->next->string;
  OS_WriteFile(output_file, entire_data);
  ReleaseScratch(scratch);
 }
 else {
  fprintf(stderr, "-----------------INCORRECT USAGE-----------------\n");
  fprintf(stderr, "Usage: code_gen.exe input_file.cdgn output_file.c\n");
  fprintf(stderr, "-------------------------------------------------\n");
 }
}
