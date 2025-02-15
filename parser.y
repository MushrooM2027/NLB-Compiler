
%{
%define api.pure full
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

void yyerror(const char *s);
int yylex();
int yyparse();

ast_node *root;
%}

%union {
    int int_value;
    char *identifier;
    struct ast_node *node;
}

%token <int_value> INT
%token <identifier> IDENTIFIER
%token INPUT DISPLAY SET TO IF IS_GREATER_THAN THEN OTHERWISE REPEAT TIMES
%token EQUAL PLUS MINUS ASTERISK SLASH
%token END INVALID

%type <node> expression primary statement assignment

%%

program:
    statement { root = $1; }
    ;

statement:
    expression
    | assignment
    ;

expression:
    expression PLUS primary  { $$ = new_binary_op_node($1, $3, PLUS); }
  | expression MINUS primary { $$ = new_binary_op_node($1, $3, MINUS); }
  | expression ASTERISK primary { $$ = new_binary_op_node($1, $3, ASTERISK); }
  | expression SLASH primary { $$ = new_binary_op_node($1, $3, SLASH); }
  | primary { $$ = $1; }
;

primary:
    INT { $$ = new_int_node($1); }
  | IDENTIFIER { $$ = new_identifier_node($1); }
;

assignment:
    SET IDENTIFIER TO INT { $$ = new_assignment_node($2, $4); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}


