
# -------------------- Todos -----------------------

- [ ] Maybe make it so if any part of the stage fails, it continues to parse error messages
- [ ] - Specify how the language works in language.h
- [ ] - #.[col][row] syntax

# ----------------- Finished Todos-------------------
- [x] - allow @generate_loop(table) to be possible in global space
      meaning it's not exclusive to @generate
- [x] - Rework all the error messages to be more helpful
- [x] - be able to specify $table_parmas in the prelude string
- [x] - parse generator elements each by \n instead of comma so you can generate structs
- [x] - Generate some c code
- [x] - Generate enum / string tables using @generate nodes
- [x] - Support new line token
- [x] - Support number literal tokens
- [x] - support @ symbol to mean the next alpha numeric is a keyword
- [x] - fix tables to correctly parse elements
- [x] - Parse @generate
- [x] - parse @generate parameters
- [x] - parse @generate prelude
- [x] - parse generate elements
- [x] - parse @generate_loop
- [x] - Make it so generate doesn't have to know definition of a table during
			 the parsing of the syntax tree
- [x] - cleanup unexpected token error conditions into single function