

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
