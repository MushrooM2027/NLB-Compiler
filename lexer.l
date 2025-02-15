%{
#include "parser.h"
#include "lexer.h"
#include "parser.tab.h"
%}

int yylex(YYSTYPE *yylvalp);

%%

"input"         { return INPUT; }
"display"       { return DISPLAY; }
"set"           { return SET; }
"to"            { return TO; }
"if"            { return IF; }
"is greater than" { return IS_GREATER_THAN; }
"then"          { return THEN; }
"otherwise"     { return OTHERWISE; }
"repeat"        { return REPEAT; }
"times"         { return TIMES; }
"="             { return EQUAL; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return ASTERISK; }
"/"             { return SLASH; }
[0-9]+ { yylval.int_value = atoi(yytext); return INT; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.identifier = strdup(yytext); return IDENTIFIER; }
[ \t\n]+        { /* ignore whitespace */ }
.               { return INVALID; }

%%

int main(){
    yylex();
}
