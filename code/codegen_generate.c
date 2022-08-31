
Arena *gen_arena = 0;

func_ void 
Generate_TokenIter(AST_Node_Table *table, String8List *list, 
                   AST_Node_TokenIter *iter_node, String8 parse_data,
                   S32 row_index, U32 *gen_index)
{
 Token_Iter iter = iter_node->iter;
 
 for(;;) {
  Token token = GrabTokenIter(&iter);
  
  switch(token.kind) {
   default: {
    String8 token_string = SubstrSizeStr8(parse_data, token.pos, token.size);
    PushStr8List(gen_arena, list, token_string);
   } break;
   
   case Token_Kind_TableParam: {
    String8 param = SubstrSizeStr8(parse_data, token.pos + 1, token.size - 1);
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
    
    // nates: if there's an open bracket, override the row index
    {
     Token open_bracket = PeekTokenIter(&iter);
     if(row_index == -1) {
      if(open_bracket.kind != Token_Kind_OpenBracket) {
       fprintf(stderr, "Error: expected open bracket [%llu:%llu]\n", 
               open_bracket.line, open_bracket.col);
       OS_Abort();
      }
     }
     
     if(open_bracket.kind == Token_Kind_OpenBracket) {
      OmitTokenIter(&iter);
      
      Token index_literal = GrabTokenIter(&iter);
      if(!IsTokenKind(index_literal, Token_Kind_NumberLiteral)) {
       fprintf(stderr, "Error: token following indexing brackets should be a number literal [%llu:%llu]\n", index_literal.line, index_literal.col);
       OS_Abort();
      }
      
      String8 num_string = SubstrSizeStr8(parse_data, index_literal.pos, index_literal.size);
      row_index = S32FromStr8(num_string);
      if(row_index >= table->element_count) {
       fprintf(stderr, "Error: index is out of bounds [%llu:%llu]\n", index_literal.line, index_literal.col);
       OS_Abort();
      }
      Token close_bracket = GrabTokenIter(&iter);
      if(!IsTokenKind(close_bracket, Token_Kind_CloseBracket)) {
       fprintf(stderr, "Error: close brackt for bracket indexing is missing [%llu:%llu]\n", close_bracket.line, close_bracket.col);
       OS_Abort();
      }
     }
    }
    
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
     
     PushStr8List(gen_arena, list, column_node->string);
     PushStr8List(gen_arena, list, remain_str);
    }
    else {
     fprintf(stderr, "Error: table parameter doesn't exist [%llu:%llu]\n",
             token.line, token.col);
     OS_Abort();
    }
   } break;
   
   case Token_Kind_Keyword: {
    String8 keyword_string = SubstrSizeStr8(parse_data, token.pos, token.size);
    if(Str8Match(keyword_string, Str8Lit("@index"), 0)) {
     String8 gen_index_number = Str8FromU64(gen_arena, *gen_index);
     PushStr8List(gen_arena, list, gen_index_number);
    }
   } break;
   
   case Token_Kind_NewLine: {
    goto Exit_Loop;
   } break;
   
   case Token_Kind_EOF: {
    fprintf(stderr, "Error: unexpected end of file token\n");
    OS_Abort();
   } break;
  }
 }
 Exit_Loop:;
 
 PushStr8List(gen_arena, list, Str8Lit("\n"));
 
 *gen_index += 1;
}

func_ void 
Generate_GeneratorLoop(AST_Node_Table *table, String8List *list, 
                       AST_Node_GeneratorLoop *loop, String8 parse_data, 
                       U32 *gen_index)
{
 for(U32 row_index = 0; row_index < table->element_count; ++row_index) {
  for(AST_Node *node = loop->first;
      node != 0; node = node->next) {
   switch(node->kind) {
    case AST_Node_Kind_GeneratorLoop: {
     AST_Node_GeneratorLoop *sub_loop = (AST_Node_GeneratorLoop *)node->data;
     Generate_GeneratorLoop(table, list, sub_loop, parse_data, gen_index);
    } break;
    
    case AST_Node_Kind_TokenIter: {
     AST_Node_TokenIter *token_iter = (AST_Node_TokenIter *)node->data;
     Generate_TokenIter(table, list, token_iter, parse_data, row_index, gen_index);
    } break;
    
    default: {
     fprintf(stderr, "Error: unexpected ast node in generator generation\n");
     OS_Abort();
    } break;
   }
  }
 }
}

func_ void
Generate_Generator(String8List *list, AST_Node_Generator *gen,
                   String8 parse_data)
{
 AST_Node_Table *table = 0;
 // nates: get the table from table hash
 {
  String8 table_name = SubstrSizeStr8(parse_data, gen->table.pos, gen->table.size);
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
  fprintf(stderr, "Error: undefined table [%llu:%llu]\n", gen->table.line, gen->table.col);
  OS_Abort();
 }
 
 // nates: parse prelude for \ characters
 {
  ArenaTemp scratch = GetScratch(0, 0);
  Token_Iter iter = gen->prelude_iter;
  
  String8List prelude_list = {0};
  for(;;) {
   Token token = GrabTokenIter(&iter);
   if(token.kind == Token_Kind_DoubleQuote) {
    break;
   }
   else if(token.kind == Token_Kind_EOF) {
    fprintf(stderr, "Error: unexpected eof in generator element [%llu:%llu]\n", 
            token.line, token.col);
    OS_Abort();
   }
   else if(token.kind == Token_Kind_BackSlash) {
    Token next = GrabTokenIter(&iter);
    if(next.kind == Token_Kind_BackSlash) {
     PushStr8List(scratch.arena, &prelude_list, Str8Lit("\\"));
    }
    else if(next.kind == Token_Kind_Identifier) {
     String8 iden = SubstrSizeStr8(parse_data, next.pos, next.size);
     U8 first = iden.str[0];
     if(first == 'n') {
      PushStr8List(scratch.arena, &prelude_list, Str8Lit("\n"));
     }
     else if(first == 't') {
      PushStr8List(scratch.arena, &prelude_list, Str8Lit("\t"));
     }
     
     String8 remain = Str8(iden.str + 1, iden.size - 1);
     PushStr8List(scratch.arena, &prelude_list, remain);
    }
    else {
     fprintf(stderr, "Error: invalid forward slash token [%llu:%llu]", next.line, next.col);
     OS_Abort();
    }
   }
   else if(token.kind == Token_Kind_TableParam) {
    String8 param = SubstrSizeStr8(parse_data, token.pos + 1, token.size - 1);
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
    
    Token open_bracket = GrabTokenIter(&iter);
    if(open_bracket.kind != Token_Kind_OpenBracket) {
     fprintf(stderr, "Error: expected open bracket [%llu:%llu]\n", 
             open_bracket.line, open_bracket.col);
     OS_Abort();
    }
    
    Token index_literal = GrabTokenIter(&iter);
    if(!IsTokenKind(index_literal, Token_Kind_NumberLiteral)) {
     fprintf(stderr, "Error: token following indexing brackets should be a number literal [%llu:%llu]\n", index_literal.line, index_literal.col);
     OS_Abort();
    }
    String8 num_string = SubstrSizeStr8(parse_data, index_literal.pos, index_literal.size);
    U32 row_index = S32FromStr8(num_string);
    if(row_index >= table->element_count) {
     fprintf(stderr, "Error: index is out of bounds [%llu:%llu]\n", index_literal.line, index_literal.col);
     OS_Abort();
    }
    Token close_bracket = GrabTokenIter(&iter);
    if(!IsTokenKind(close_bracket, Token_Kind_CloseBracket)) {
     fprintf(stderr, "Error: closing index bracket missing [%llu:%llu]\n", close_bracket.line, close_bracket.col);
     OS_Abort();
    }
    
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
     
     PushStr8List(gen_arena, &prelude_list, column_node->string);
     PushStr8List(gen_arena, &prelude_list, remain_str);
    }
    else {
     fprintf(stderr, "Error: table parameter doesn't exist [%llu:%llu]\n",
             token.line, token.col);
     OS_Abort();
    }
   }
   else {
    String8 token_string = SubstrSizeStr8(parse_data, token.pos, token.size);
    PushStr8List(scratch.arena, &prelude_list, token_string);
   }
  }
  
  String8 full = JoinStr8List(gen_arena, &prelude_list, 0);
  PushStr8List(gen_arena, list, full);
  ReleaseScratch(scratch);
 }
 
 
 PushStr8List(gen_arena, list, Str8Lit("\n"));
 PushStr8List(gen_arena, list, Str8Lit("{"));
 PushStr8List(gen_arena, list, Str8Lit("\n"));
 
 B32 gen_index = 0;
 for(AST_Node *node = gen->first;
     node != 0; node = node->next) {
  switch(node->kind) {
   case AST_Node_Kind_GeneratorLoop: {
    AST_Node_GeneratorLoop *loop = (AST_Node_GeneratorLoop *)node->data;
    Generate_GeneratorLoop(table, list, loop, parse_data, &gen_index);
   } break;
   
   case AST_Node_Kind_TokenIter: {
    AST_Node_TokenIter *token_iter = (AST_Node_TokenIter *)node->data;
    Generate_TokenIter(table, list, token_iter, parse_data, -1, &gen_index);
   } break;
   
   default: {
    fprintf(stderr, "Error: unexpected ast node in generator generation\n");
    OS_Abort();
   } break;
  }
 }
 PushStr8List(gen_arena, list, Str8Lit("}"));
 PushStr8List(gen_arena, list, Str8Lit(";"));
 PushStr8List(gen_arena, list, Str8Lit("\n"));
 PushStr8List(gen_arena, list, Str8Lit("\n"));
}

func_ String8 
Generate_AST(AST ast, String8 parse_data)
{
 String8List data_list = {0};
 for(AST_Node *node = ast.first;
     node != 0; node = node->next) {
  switch(node->kind) {
   case AST_Node_Kind_Table: {
    // nates: skip over tables
    // TODO(nates): Does the ast really need to store these nods if we could 
    // just store the table into our table hash instead?
   } break;
   
   case AST_Node_Kind_Generator: {
    AST_Node_Generator *gen = (AST_Node_Generator *)node->data;
    Generate_Generator(&data_list, gen, parse_data);
   } break;
   
   default: {
    fprintf(stderr, "Error: unexpected ast node in main generation\n");
    OS_Abort();
   } break;
  }
 }
 
 String8 data = JoinStr8List(gen_arena, &data_list, 0);
 return(data);
}