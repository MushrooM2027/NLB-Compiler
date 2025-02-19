%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

int yylex();
void yyerror(const char *s);

ASTNode *root;

int evaluate_ast(ASTNode *node);


%}

%left PLUS MINUS
%left ASTERISK SLASH

%union {
    char *op_value;
    struct ASTNode *node;
    int int_value;
    float float_value;
}

%token SET
%token TO
%token <int_value> NUMBER
%token <float_value> FLOAT
%token <op_value> IDENTIFIER
%token PLUS '+'
%token MINUS '-'
%token ASTERISK '*'
%token SLASH '/'


%type <node> program statement expression term factor

%start program

%%

program:
    statement { root = $1; }
    ;

statement:
    IDENTIFIER TO expression { 
        $$ = new_assignment_node($1, $3);  // Store the assignment properly
    }
    | expression { $$ = $1; }
    ;


expression:
    expression PLUS expression { $$ = new_binary_op_node($1, $3, '+', ($1->is_float || $3->is_float)); }
    | expression MINUS expression { $$ = new_binary_op_node($1, $3, '-', ($1->is_float || $3->is_float)); }
    | term { $$ = $1;}
    ;

term:
    term ASTERISK factor { $$ = new_binary_op_node($1, $3, '*', ($1->is_float || $3->is_float)); }
    | term SLASH factor { $$ = new_binary_op_node($1, $3, '/', ($1->is_float || $3->is_float)); }
    | factor {$$ = $1;}
    ;

factor:
    NUMBER { $$ = new_number_node($1,0); }
    | FLOAT { $$ = new_float_node($1,1);}
    | IDENTIFIER { $$ = new_identifier_node($1); }
    | '(' expression ')' { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    printf("Error: %s\n", s);
}

ASTNode *get_root(){
    return root;
}
