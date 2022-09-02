
Arena *gen_arena = 0;

func_ void 
Generate_TableParam(String8List *output, AST_Node_Table *table, 
                    Token_Iter *iter, String8 input, S32 row_index,
                    Token table_param)
{
 String8 param = SubSizeStr8(input, table_param.pos + 1, table_param.size - 1);
 String8 remain_str = {0};
 
 U32 col_index = 0;
 B32 param_found = false;
 for(String8Node *table_param = table->parameters.first;
     table_param != 0; table_param = table_param->next) {
  String8 found = FindFirstStr8(param, table_param->string, 0);
  if(found.size) {
   param_found = true;
   remain_str.str = found.str + found.size;
   remain_str.size = param.size - found.size;
   break;
  }
  col_index++;
 }
 
 // nates: if the row index is -1, expect bracket indexing, else skip parsing that
 Token open_bracket = PeekTokenIter(iter);
 if(row_index == -1) {
  if(open_bracket.kind != Token_Kind_OpenBracket) {
   fprintf(stderr, "Error: expected open bracket [%llu:%llu]\n", 
           open_bracket.line, open_bracket.col);
   OS_Abort();
  }
 }
 
 if(open_bracket.kind == Token_Kind_OpenBracket) {
  OmitTokenIter(iter);
  
  Token index_literal = GrabTokenIter(iter);
  if(index_literal.kind != Token_Kind_NumberLiteral) {
   fprintf(stderr, "Error: token following indexing brackets should be a number literal [%llu:%llu]\n", index_literal.line, index_literal.col);
   OS_Abort();
  }
  String8 num_string = SubSizeStr8(input, index_literal.pos, index_literal.size);
  row_index = S32FromStr8(num_string);
  if(row_index >= table->element_count) {
   fprintf(stderr, "Error: index is out of bounds [%llu:%llu]\n", index_literal.line, index_literal.col);
   OS_Abort();
  }
  Token close_bracket = GrabTokenIter(iter);
  if(close_bracket.kind != Token_Kind_CloseBracket) {
   fprintf(stderr, "Error: closing index bracket missing [%llu:%llu]\n", close_bracket.line, close_bracket.col);
   OS_Abort();
  }
 }
 
 Assert(row_index > -1);
 
 if(param_found) {
  Table_Element *row_element = table->first;
  for(U32 r = 0; r < row_index; ++r) {
   row_element = row_element->next;
   if(row_element == 0) {
    InvalidPath;
   }
  }
  
  String8Node *column_node = row_element->list.first;
  for(U32 c = 0; c < col_index; ++c) {
   column_node = column_node->next;
   if(column_node == 0) {
    InvalidPath;
   }
  }
  
  PushStr8List(gen_arena, output, column_node->string);
  PushStr8List(gen_arena, output, remain_str);
 }
 else {
  ArenaTemp scratch = GetScratch(0, 0);
  String8 table_name_cstr = CopyStr8(scratch.arena, table->name);
  String8 table_param_cstr = CopyStr8(scratch.arena, param);
  fprintf(stderr, "Error: param(%s) doesn't exist for table(%s) [%llu:%llu]\n",
          table_param_cstr.str, table_name_cstr.str,
          table_param.line, table_param.col);
  ReleaseScratch(scratch);
  OS_Abort();
 }
}



func_ void 
Generate_Loop(String8List *output, AST_Node_Table *table, 
              Token_Iter *iter, String8 input, S64 *gen_index)
{
 // nates: parse the open brace
 {
  SkipWhitespaceIter(iter);
  Token open_brace = GrabTokenIter(iter);
  if(open_brace.kind != Token_Kind_OpenBrace) {
   fprintf(stderr, "Error: missing open brace after @generate_loop [%llu:%llu]\n", open_brace.line, open_brace.col);
   OS_Abort();
  }
 }
 
 Token_Iter_Node *first = 0;
 Token_Iter_Node *last = 0;
 // nates: parse all the tokens into iterators and parse the close bracket
 
 {
  for(;;) {
   SkipWhitespaceIter(iter);
   Token loop_token = PeekTokenIter(iter);
   if(loop_token.kind == Token_Kind_CloseBrace) {
    OmitTokenIter(iter);
    break;
   }
   
   Token_Iter_Node *iter_node = PushArray(gen_arena, Token_Iter_Node, 1);
   QueuePush(first, last, iter_node);
   iter_node->iter = *iter;
   // nates: skip tokens till new line
   for(;;) {
    Token skip = GrabTokenIter(iter);
    if(skip.kind == Token_Kind_NewLine) {
     break;
    }
   }
  }
 }
 
 // nates: start looping over the the iterators and row indices
 // and generate some code
 for(U32 row_index = 0; row_index < table->element_count; ++row_index) {
  for(Token_Iter_Node *iter_node = first; iter_node != 0; iter_node = iter_node->next) {
   Token_Iter loop_iter = iter_node->iter;
   for(;;) {
    Token loop_token = GrabTokenIter(&loop_iter);
    if(loop_token.kind == Token_Kind_Keyword) {
     String8 loop_token_string = SubSizeStr8(input, loop_token.pos, loop_token.size);
     if(Str8Match(loop_token_string, Str8Lit("@generate_block"), 0)) {
      fprintf(stderr, "Error: @generate_block keyword not allowed in a @generate_loop [%llu:%llu]\n", loop_token.line, loop_token.col);
      OS_Abort();
     }
     else if(Str8Match(loop_token_string, Str8Lit("@generate_loop"), 0)) {
      Generate_Loop(output, table, &loop_iter, input, gen_index);
     }
     else if(Str8Match(loop_token_string, Str8Lit("@index"), 0)) {
      String8 number_string = Str8FromS64(gen_arena, *gen_index);
      PushStr8List(gen_arena, output, number_string);
     }
     else if(Str8Match(loop_token_string, Str8Lit("@inc"), 0)) {
      *gen_index += 1;
     }
     else if(Str8Match(loop_token_string, Str8Lit("@dec"), 0)) {
      *gen_index -= 1;
     }
     else {
      ArenaTemp scratch = GetScratch(0, 0);
      String8 keyword_cstr = CopyStr8(scratch.arena, loop_token_string);
      fprintf(stderr, "Error: invalid keyword(%s) [%llu:%llu]\n", 
              keyword_cstr.str,
              loop_token.line, loop_token.col);
      ReleaseScratch(scratch);
      OS_Abort();
     }
    }
    else if(loop_token.kind == Token_Kind_TableParam) {
     Generate_TableParam(output, table, &loop_iter, input, row_index, loop_token);
    }
    else if(loop_token.kind == Token_Kind_NewLine) {
     PushStr8List(gen_arena, output, Str8Lit("\n"));
     break;
    }
    else if(loop_token.kind == Token_Kind_EOF) {
     break;
    }
    else{
     String8 loop_token_string = SubSizeStr8(input, loop_token.pos, loop_token.size);
     PushStr8List(gen_arena, output, loop_token_string);
    }
   }
   (*gen_index) += 1;
  }
 }
 
 // nates: parse the remaining comma on our main token iterator
 {
  SkipWhitespaceIter(iter);
  Token token = GrabTokenIter(iter);
  if(token.kind != Token_Kind_Comma) {
   fprintf(stderr, "Error: @generate_loop must end with a comma after braces [%llu:%llu])\n", token.line, token.col);
   OS_Abort();
  }
 }
 // nates: skip any tokens until new line and make sure 
 // only whitespace is allowed
 {
  for(;;) {
   Token token = GrabTokenIter(iter);
   if(token.kind == Token_Kind_NewLine) {
    break;
   }
   else if(token.kind != Token_Kind_Whitespace) {
    fprintf(stderr, "Error: tokens after ending loop brace must only be whitespcae [%llu:%llu]\n", token.line, token.col);
    OS_Abort();
   }
  }
 }
 
}


func_ void 
Generate_Block(String8List *output, AST_Node_GenerateBlock *gen, String8 input)
{
 AST_Node_Table *table = 0;
 // nates: get the table from table hash
 {
  String8 table_name = SubSizeStr8(input, gen->table_name.pos, gen->table_name.size);
  U64 hash = HashStr8(table_name);
  U64 slot = hash % ArrayCount(ast_state.table_nodes);
  for(AST_TableHashNode *node = ast_state.table_nodes[slot];
      node != 0; node = node->hash_next)
  {
   if(Str8Match(node->table_name, table_name, 0)) {
    table = node->table;
    break;
   }
  }
 }
 
 if(table == 0)
 {
  ArenaTemp scratch = GetScratch(0, 0);
  String8 table_name = SubSizeStr8(input, gen->table_name.pos, gen->table_name.size);
  String8 table_name_cstr = CopyStr8(scratch.arena, table_name);
  fprintf(stderr, "Error: undefined table(%s) [%llu:%llu]\n", 
          table_name_cstr.str,
          gen->table_name.line, gen->table_name.col);
  ReleaseScratch(scratch);
  OS_Abort();
 }
 
 S64 gen_index = 0;
 {
  ArenaTemp scratch = GetScratch(0, 0);
  Token_Iter iter = gen->start;
  
  // nates: loop over all the tokens in the block and output them to the block list
  String8List block_list = {0};
  for(;;) {
   Token token = GrabTokenIter(&iter);
   if(token.kind == Token_Kind_Keyword) {
    String8 keyword_string = SubSizeStr8(input, token.pos, token.size);
    if(Str8Match(keyword_string, Str8Lit("@generate_block"), 0)) {
     break;
    }
    else if(Str8Match(keyword_string, Str8Lit("@generate_loop"), 0)) {
     Generate_Loop(&block_list, table, &iter, input, &gen_index);
    }
    else if(Str8Match(keyword_string, Str8Lit("@index"), 0)) {
     String8 number_string = Str8FromS64(gen_arena, gen_index);
     PushStr8List(gen_arena, &block_list, number_string);
    }
    else if(Str8Match(keyword_string, Str8Lit("@inc"), 0)) {
     gen_index++;
    }
    else if(Str8Match(keyword_string, Str8Lit("@dec"), 0)) {
     gen_index--;
    }
   }
   else if(token.kind == Token_Kind_EOF) {
    break;
   }
   else if(token.kind == Token_Kind_TableParam) {
    Generate_TableParam(&block_list, table, &iter, input, -1, token);
   }
   else {
    String8 token_string = SubSizeStr8(input, token.pos, token.size);
    PushStr8List(scratch.arena, &block_list, token_string);
   }
  }
  
  String8 full = JoinStr8List(gen_arena, &block_list, 0);
  PushStr8List(gen_arena, output, full);
  ReleaseScratch(scratch);
 }
}

func_ String8 
Generate_AST(AST ast, String8 input)
{
 String8List output = {0};
 for(AST_Node *node = ast.first;
     node != 0; node = node->next) {
  switch(node->kind) {
   case AST_Node_Kind_Table: {
    // nates: skip over tables
    // TODO(nates): Does the ast really need to store these nods if we could 
    // just store the table into our table hash instead?
   } break;
   
   case AST_Node_Kind_GenerateBlock: {
    AST_Node_GenerateBlock *gen = (AST_Node_GenerateBlock *)node->data;
    Generate_Block(&output, gen, input);
   } break;
   
   default: {
    fprintf(stderr, "Error: unexpected ast node in main generation\n");
    OS_Abort();
   } break;
  }
 }
 
 String8 data = JoinStr8List(gen_arena, &output, 0);
 return(data);
}