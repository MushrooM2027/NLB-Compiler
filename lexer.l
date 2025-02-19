%{

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.tab.h"

char *create_string(const char *value, int length){
    char *return_value = (char *)malloc(length + 1);
    if (return_value != NULL) {
        strncpy(return_value, value, length);
        return_value[length] = '\0';
    }
    return return_value;
}

%}

%%

"set"           { return SET; }
"to"            { return TO; }
[0-9]+\.[0-9]+  { yylval.float_value = atof(yytext); return FLOAT; }
[0-9]+          {yylval.int_value = atoi(yytext); return NUMBER;}
[a-zA-Z_][a-zA-Z0-9_]*  {yylval.op_value = create_string(yytext, yyleng); return IDENTIFIER;}
"+"             {return PLUS;}
"-"             { return MINUS; }
"*"             { return ASTERISK; }
"/"             { return SLASH; }
"("             { return '('; }
")"             { return ')'; }
[ \t\n]+        {}
.               { return yytext[0]; }

%%
