#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define DS_IMPLEMENTATION
#include "ds.h"
#include <stdio.h>

enum token_type {
    INPUT,
    DISPLAY,
    IDENTIFIER,
    INT,
    EQUAL,
    INVALID,
    END,
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    SET,
    TO,
    IF,
    IS_GREATER_THAN,
    THEN,
    OTHERWISE,
    REPEAT,
    TIMES,
};

struct token {
    enum token_type type;
    char *literal;
};

const char *token_type_names(enum token_type type){
    switch (type)
    {
    case INPUT:
        return "INPUT";
    case DISPLAY:
        return "DISPLAY";
    case IDENTIFIER:
        return "IDENTIFIER";
    case INT:
        return "INT";
    case EQUAL:
        return "EQUAL";
    case PLUS:
        return "PLUS";
    case MINUS:
        return "MINUS";
    case ASTERISK:
        return "ASTERISK";
    case SLASH:
        return "SLASH";
    case INVALID:    
        return "INVALID";
    case END:
        return "END";
    default:
        return "UNKNOWN";
    }
};

void print_token(struct token *t){
    const char *type = token_type_names(t->type);
    if(t->literal != NULL){
        printf("Token: %s, Literal: %s\n", type, t->literal);
    }else {
        printf("Token: %s\n", type);
    }
    // printf("\n");

}

struct lexer {
    char *buffer;
    unsigned int buffer_len;
    unsigned int pos;
    unsigned int read_pos;
    char ch;
};

static char lexer_peek_char(struct lexer *l) {
    if (l->read_pos >= l->buffer_len) {
        return EOF;
    }

    return l->buffer[l->read_pos];
}

static char lexer_read_char(struct lexer *l) {
    l->ch = lexer_peek_char(l);

    l->pos = l->read_pos;
    l->read_pos += 1;

    return l->ch;
}

static void skip_whitespaces(struct lexer *l) {
    while (isspace(l->ch)) {
        lexer_read_char(l);
    }
}

static void lexer_init(struct lexer *l, char *buffer, unsigned int buffer_len) {
    l->buffer = buffer;
    l->buffer_len = buffer_len;
    l->pos = 0;
    l->read_pos = 0;
    l->ch = 0;

    lexer_read_char(l);
}

static struct token lexer_next_token(struct lexer *l) {
    skip_whitespaces(l);

    // printf("Current char: %c\n", l->ch);

    if(l->ch == EOF){
        lexer_read_char(l);
        return (struct token){.type = END, .literal = NULL};
    } else if(l->ch == '='){
        lexer_read_char(l);
        return (struct token){.type = EQUAL, .literal = NULL};
    }
    else if(l->ch == '+'){
        lexer_read_char(l);
        return (struct token){.type = PLUS, .literal = NULL};
    }
    else if(l->ch == '-'){
        lexer_read_char(l);
        return (struct token){.type = MINUS, .literal = NULL};
    }
    else if(l->ch == '*'){
        lexer_read_char(l);
        return (struct token){.type = ASTERISK, .literal = NULL};
    }
    else if(l->ch == '/'){
        lexer_read_char(l);
        return (struct token){.type = SLASH, .literal = NULL};
    }
    else if(isdigit(l->ch)){
        unsigned int start = l->pos;
        while(isdigit(l->ch)){
            lexer_read_char(l);
        }
        ds_string_slice slice = {.str = l->buffer + start, .len = l->pos - start};
        char *value = NULL;
        ds_string_slice_to_owned(&slice, &value);
        return (struct token){.type = INT, .literal = value};
    }
    else if(isalpha(l->ch) || l->ch == '-'){
        unsigned int start = l->pos;
        while(isalpha(l->ch) || l->ch == '-'){
            lexer_read_char(l);
        }
        ds_string_slice slice = {.str = l->buffer + start, .len = l->pos - start};
        char *value = NULL;
        ds_string_slice_to_owned(&slice, &value);
        if (strcmp(value, "input") == 0) {
        return (struct token){.type = INPUT, .literal = NULL}; 
        } else if (strcmp(value, "display") == 0) {
        return (struct token){.type = DISPLAY, .literal = NULL}; 
        } else if (strcmp(value, "set") == 0) {
        return (struct token){.type = SET, .literal = NULL}; 
        } else if (strcmp(value, "to") == 0) {
        return (struct token){.type = TO, .literal = NULL}; 
        } else if (strcmp(value, "if") == 0) {
        return (struct token){.type = IF, .literal = NULL}; 
        } else if (strcmp(value, "is greater than") == 0) {
        return (struct token){.type = IS_GREATER_THAN, .literal = NULL}; 
        } else if (strcmp(value, "then") == 0) {
        return (struct token){.type = THEN, .literal = NULL}; 
        } else if (strcmp(value, "otherwise") == 0) {
        return (struct token){.type = OTHERWISE, .literal = NULL}; 
        } else if (strcmp(value, "repeat") == 0) {
        return (struct token){.type = REPEAT, .literal = NULL}; 
        } else if (strcmp(value, "times") == 0) {
        return (struct token){.type = TIMES, .literal = NULL}; 
        } else {
        return (struct token){.type = IDENTIFIER, .literal = value};
    }
}
    else {
        ds_string_slice slice = {.str = l->buffer + l->pos, .len = 1};
        char *value = NULL;
        ds_string_slice_to_owned(&slice, &value);
        lexer_read_char(l);
        return (struct token){.type = INVALID, .literal = value};
    }
};

int lexer_tokenizer(char *buffer, unsigned int buffer_len,ds_dynamic_array *tokens){
    struct lexer l;
    lexer_init(&l, buffer, buffer_len);
    
    struct token t;
    do{
        t = lexer_next_token(&l);
        if(ds_dynamic_array_append(tokens, &t) != 0){
            DS_PANIC("Failed to append token to array");
        }
    }while (t.type != END);

    return 0;
};
    

int main() {
    char *buffer = "input x = 5 + 5 define y = 10 * 10 display x display y";
    int length = strlen(buffer);

    ds_dynamic_array tokens;
    ds_dynamic_array_init(&tokens, sizeof(struct token));

    lexer_tokenizer(buffer, length, &tokens);
    for (unsigned int i=0; i < tokens.count; i++){
        struct token *t = NULL;
        ds_dynamic_array_get_ref(&tokens, i, (void **)&t);
        print_token(t);
    }

    printf("Buffer: %s\n", buffer);
}
