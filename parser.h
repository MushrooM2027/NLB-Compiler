#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

int yyparse();
ASTNode *get_root();  // Function to return the AST root

#endif
