
////////////////////////////////////
// nates: globals

global_ AST_State  ast_state = {false};
global_ Arena *ast_arena;

////////////////////////////////////////
// nates: Functions

func_ void
Parse_TableElements(Token_Iter *iter, String8 data, AST_Node_Table *table, B32 brace_required)
{
 for(;;) {
  SkipWhitespaceIter(iter);
  B32 close_brace_required = false;
  Token element_start = PeekTokenIter(iter);
  if(IsTokenKind(element_start, Token_Kind_CloseBrace)) {
   break;
  }
  
  if(brace_required) {
   if(!IsTokenKind(element_start, Token_Kind_OpenBrace)) {
    fprintf(stderr, "Error: expected compound table element starting with \'{\' [%llu:%llu]\n", element_start.line, element_start.col);
    OS_Abort();
   }
  }
  
  if(element_start.kind == Token_Kind_OpenBrace) {
   close_brace_required = true;
   OmitTokenIter(iter);
  }
  
  Table_Element *table_element = PushArray(ast_arena, Table_Element, 1);
  table->element_count++;
  QueuePush(table->first, table->last, table_element);
  
  for(;;) {
   SkipWhitespaceIter(iter);
   // nates: store string into table element
   Token token = GrabTokenIter(iter);
   if(token.kind == Token_Kind_Identifier ||
      token.kind == Token_Kind_NumberLiteral) {
    String8 element_string = SubstrSizeStr8(data, token.pos, token.size);
    PushStr8List(ast_arena, &table_element->list, element_string);
   }
   else if(token.kind == Token_Kind_CloseBrace) {
    if(close_brace_required == false) {
     fprintf(stderr, "Error: unnecessary close brace before comma [%llu:%llu]\n", token.line, token.col);
     OS_Abort();
    }
    
    Token comma = GrabTokenIter(iter);
    if(!IsTokenKind(comma, Token_Kind_Comma)) {
     fprintf(stderr, "Error: compound elements must be separated by a comma [%llu:%llu]\n", comma.line, comma.col);
     OS_Abort();
    }
    break;
   }
   else if(token.kind == Token_Kind_Comma) {
    if(close_brace_required) {
     fprintf(stderr, "Error: compound element must end with \'}\' before comma [%llu:%llu]\n", token.line, token.col);
     OS_Abort();
    }
    break;
   }
   else {
    fprintf(stderr, "Error: expected identifier or str/num literal [%llu:%llu]\n", token.line, token.col);
    OS_Abort();
   }
   
  }
  if(table_element->list.count != table->parameters.count) {
   fprintf(stderr, "Error: table element count(%llu) dosen't match table parameter count(%llu) [%llu:%llu]\n", 
           table_element->list.count, table->parameters.count, element_start.line, element_start.col);
   OS_Abort();
  }
  
 }
}

func_ void
Parse_Table(AST *ast, Token_Iter *iter, String8 data)
{
 // nates: create a new table node and push it to the ast
 AST_Node *node = PushArray(ast_arena, AST_Node, 1);
 QueuePush(ast->first, ast->last, node);
 ast->count += 1;
 node->kind = AST_Node_Kind_Table;
 node->data = PushSize(ast_arena, sizeof(AST_Node_Table));
 AST_Node_Table *table = (AST_Node_Table *)node->data;
 
 // nates: grab the first token; assert; store into hash
 {
  Token token = GrabTokenIter(iter);
  if(!IsTokenKind(token, Token_Kind_Identifier)) {
   fprintf(stderr, "Error: fatal error, parser expected a table identifier token [%llu:%llu]\n", token.line, token.col);
   OS_Abort();
  }
  
  String8 table_name = SubstrSizeStr8(data, token.pos, token.size);
  
  // nates: store the table into table_hash using it's name
  table->name = table_name;
  U64 hash = HashStr8(table_name);
  U64 slot = hash % ArrayCount(ast_state.table_nodes);
  AST_TableHashNode *hash_node = PushArray(ast_arena, AST_TableHashNode, 1);
  hash_node->table_name = table_name;
  hash_node->table = table;
  
  StackPushN(ast_state.table_nodes[slot], hash_node, hash_next);
 }
 
 // nates: consume the open parenthesis
 {
  SkipWhitespaceIter(iter);
  Token token = GrabTokenIter(iter);
  if(!IsTokenKind(token, Token_Kind_OpenParen)) {
   fprintf(stderr, "Error: token wasn't open parenthesis after table identifier [%llu:%llu]\n", token.line, token.col);
   OS_Abort();
  }
 }
 
 // nates: parse parameters into *table
 {
  SkipWhitespaceIter(iter);
  // nates: parameter loop
  for(;;) {
   Token token = GrabTokenIter(iter);
   if(!IsTokenKind(token, Token_Kind_Identifier)) {
    fprintf(stderr, "Error: expected a table parameter identifier token [%llu:%llu]\n", token.line, token.col);
    OS_Abort();
   }
   
   // nates: push parameter onto table
   String8 param_string = SubstrSizeStr8(data, token.pos, token.size);
   PushStr8List(ast_arena, &table->parameters, param_string);
   
   // nates: check for comma or close parenthesis
   Token next_token = GrabTokenIter(iter);
   if(next_token.kind == Token_Kind_Comma) {
    // nates: continue down
   }
   else if(next_token.kind == Token_Kind_CloseParen) {
    break;
   }
   else {
    fprintf(stderr, "Error: expected comma / closing parenthesis after table parameter [%llu:%llu]\n", next_token.line, next_token.col);
    OS_Abort();
   }
   
   SkipWhitespaceIter(iter);
  }
 }
 
 // nates: assert and consume the open brace
 {
  SkipWhitespaceIter(iter);
  Token token = GrabTokenIter(iter);
  if(!IsTokenKind(token, Token_Kind_OpenBrace)) {
   fprintf(stderr, "Error: an open brace \'{\' must come after table parameter closing parenthesis\n", token.line, token.col);
   OS_Abort();
  }
 }
 
 // nates: parse multi/single string elements into the table
 {
  SkipWhitespaceIter(iter);
  if(table->parameters.count == 1) {
   Parse_TableElements(iter, data, table, false);
  }
  else {
   Parse_TableElements(iter, data, table, true);
  }
 }
 
 // nates: assert and consume the close brace
 {
  SkipWhitespaceIter(iter);
  Token token = GrabTokenIter(iter);
  if(!IsTokenKind(token, Token_Kind_CloseBrace)) { 
   fprintf(stderr, "Error: expected closing brace \'}\' at the end of parsing a table\n", token.line, token.col);
   OS_Abort();
  }
 }
}

func_ void
Parse_GeneratorElement(AST_Node *node, Token_Iter *iter, String8 data)
{
 node->kind = AST_Node_Kind_TokenIter;
 node->data = PushSize(ast_arena, sizeof(AST_Node_TokenIter));
 AST_Node_TokenIter *token_iter = (AST_Node_TokenIter *)node->data;
 token_iter->iter = *iter;
 for(;;) {
  Token token = GrabTokenIter(iter);
  if(token.kind == Token_Kind_NewLine) {
   break;
  }
 }
 
}


func_ void 
Parse_GeneratorLoop(AST_Node *node, Token_Iter *iter, String8 data)
{
 node->kind = AST_Node_Kind_GeneratorLoop;
 node->data = PushSize(ast_arena, sizeof(AST_Node_GeneratorLoop));
 AST_Node_GeneratorLoop *loop = (AST_Node_GeneratorLoop *)node->data;
 
 // nates: consume keyword
 {
  Token token = GrabTokenIter(iter);
  if(!IsTokenKind(token, Token_Kind_Keyword)) {
   fprintf(stderr, "Fatal Error: loop parser didn't have a @keyword token as it's first token\n", token.line, token.col);
   OS_Abort();
  }
 }
 
 // nates: consume open brace
 {
  SkipWhitespaceIter(iter);
  Token token = GrabTokenIter(iter);
  if(!IsTokenKind(token, Token_Kind_OpenBrace)) {
   fprintf(stderr, "Error: expected an open brace at the beginning of a @generate_loop [%llu:%llu]\n", token.line, token.col);
   OS_Abort();
  }
 }
 
 // nates: parse elements / loops
 for(;;) {
  // nates: parse single elements or generate loops
  // single elements must start as an identifier, string literal, or number literal
  // anything after the start is allowed, i.e. "identifier = (1 << @index)"
  SkipWhitespaceIter(iter);
  Token token = PeekTokenIter(iter);
  Parse_Kind parse_kind = Parse_Kind_Null;
  if(token.kind == Token_Kind_Keyword) {
   String8 keyword_string = SubstrSizeStr8(data, token.pos, token.size);
   if(Str8Match(keyword_string, Str8Lit("@generate_loop"), 0)) {
    parse_kind = Parse_Kind_Loop;
   }
   else if(Str8Match(keyword_string, Str8Lit("@index"), 0)) {
    parse_kind = Parse_Kind_Elements;
   }
  }
  else if(token.kind == Token_Kind_CloseBrace) {
   OmitTokenIter(iter);
   Token comma = GrabTokenIter(iter);
   if(!IsTokenKind(comma, Token_Kind_Comma)) {
    fprintf(stderr, "Error: expected a comma after @generate_loop closing brace [%llu:%llu]\n", comma.line, comma.col);
    OS_Abort();
   }
   break;
  }
  else {
   parse_kind = Parse_Kind_Elements;
  }
  
  AST_Node *node = 0;
  if(parse_kind != Parse_Kind_Null) {
   node = PushArray(ast_arena, AST_Node, 1);
   QueuePush(loop->first, loop->last, node);
  }
  
  // nates: parse either an element or a generate_loop
  if(parse_kind == Parse_Kind_Elements) {
   Parse_GeneratorElement(node, iter, data);
  }
  else if(parse_kind == Parse_Kind_Loop) {
   Parse_GeneratorLoop(node, iter, data);
  }
  else {
   fprintf(stderr, "Error: unable to parse generator element [%llu:%llu]\n", token.line, token.col);
   OS_Abort();
  }
 }
}

func_ void
Parse_Generator(AST *ast, Token_Iter *iter, String8 data)
{
 // nates: create a new generator node and push it to the ast
 AST_Node *node = PushArray(ast_arena, AST_Node, 1);
 QueuePush(ast->first, ast->last, node);
 ast->count += 1;
 node->kind = AST_Node_Kind_Generator;
 node->data = PushSize(ast_arena, sizeof(AST_Node_Generator));
 AST_Node_Generator *gen = (AST_Node_Generator *)node->data;
 
 // nates: parse the keyword
 {
  Token token = GrabTokenIter(iter);
  if(!IsTokenKind(token, Token_Kind_Keyword)) {
   fprintf(stderr, "Fatal Error: generator parser failed to recieve a keyword token [%llu:%llu]\n", token.line, token.col);
   OS_Abort();
  }
 }
 
 // nates: parse open parenthesis
 {
  SkipWhitespaceIter(iter);
  Token token = GrabTokenIter(iter);
  if(!IsTokenKind(token, Token_Kind_OpenParen)) {
   fprintf(stderr, "Error: open parentheses should follow @generate [%llu:%llu]\n", token.line, token.col);
   OS_Abort();
  }
 }
 
 // nates: get the input table information
 {
  SkipWhitespaceIter(iter);
  Token token = GrabTokenIter(iter);
  if(!IsTokenKind(token, Token_Kind_Identifier)) {
   fprintf(stderr, "Error: a table identifier should follow open parethesis of @generate [%llu:%llu]\n", token.line, token.col);
   OS_Abort();
  }
  gen->table = token;
 }
 
 // nates: parse closing parenthesis
 {
  SkipWhitespaceIter(iter);
  Token token = GrabTokenIter(iter);
  if(!IsTokenKind(token, Token_Kind_CloseParen)) {
   fprintf(stderr, "Error: closing parenthesis of @generate is missing [%llu:%llu]\n", token.line, token.col);
   OS_Abort();
  }
 }
 
 // nates: parse generator elements / loops
 {
  for(;;)
  {
   SkipWhitespaceIter(iter);
   Token token = PeekTokenIter(iter);
   Parse_Kind parse_kind = Parse_Kind_Null;
   if(token.kind == Token_Kind_Keyword) {
    String8 keyword_string = SubstrSizeStr8(data, token.pos, token.size);
    if(Str8Match(keyword_string, Str8Lit("@generate_loop"), 0)) {
     parse_kind = Parse_Kind_Loop;
    }
    else if(Str8Match(keyword_string, Str8Lit("@index"), 0)) {
     parse_kind = Parse_Kind_Elements;
    }
   }
   else if(token.kind == Token_Kind_CloseBrace) {
    OmitTokenIter(iter);
    break;
   }
   else {
    parse_kind = Parse_Kind_Elements;
   }
   
   AST_Node *node = 0;
   if(parse_kind != Parse_Kind_Null) {
    node = PushArray(ast_arena, AST_Node, 1);
    QueuePush(gen->first, gen->last, node);
   }
   
   if(parse_kind == Parse_Kind_Elements) {
    Parse_GeneratorElement(node, iter, data);
   }
   else if(parse_kind == Parse_Kind_Loop) {
    Parse_GeneratorLoop(node, iter, data);
   }
   else {
    fprintf(stderr, "Error: unable to parse generator element [%llu:%llu]\n", token.line, token.col);
    OS_Abort();
   }
  }
 }
}

func_ AST 
Parse_Tokens(Token_Iter *iter, String8 data)
{
 AST ast = {0};
 for(;;)
 {
  SkipWhitespaceIter(iter);
  // nates: peek token and either parse as an identifier or as a generator
  Token token = PeekTokenIter(iter);
  if(token.kind == Token_Kind_Identifier) {
   Parse_Table(&ast, iter, data);
  }
  else if(token.kind == Token_Kind_Keyword) {
   String8 keyword = SubstrSizeStr8(data, token.pos, token.size);
   if(Str8Match(keyword, Str8Lit("@generate"), 0)) {
    Parse_Generator(&ast, iter, data);
   }
   else {
    fprintf(stderr, "Error: invalid @keyword [%llu:%llu]\n", token.line, token.col);
    OS_Abort();
   }
  }
  else if(token.kind == Token_Kind_EOF) {
   break;
  }
  else {
   fprintf(stderr, "Error: expected identifier or keyword [%llu:%llu]\n", token.line, token.col);
   OS_Abort();
  }
 }
 
 return(ast);
}
