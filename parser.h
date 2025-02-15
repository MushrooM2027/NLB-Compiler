#ifndef PARSER_H
#define PARSER_H
#include "parser.tab.h"
#include "lexer.h"

// AST node types
typedef enum {
    NODE_INT,
    NODE_BINARY_OP,
    NODE_IDENTIFIER,
    NODE_ASSIGNMENT
} node_type;

// AST node structure
typedef struct ast_node {
    node_type type;
    union {
        int int_value;
        struct {
            struct ast_node *left;
            struct ast_node *right;
            int op; // Change token_type to int
        } binary_op;
        char *identifier;
        struct {
            char *identifier;
            int value;
        } assignment;
    } data;
} ast_node;

// Function declarations
ast_node *new_int_node(int value);
ast_node *new_identifier_node(char *identifier);
ast_node *new_binary_op_node(ast_node *left, ast_node *right, int op); // Change token_type to int
ast_node *new_assignment_node(char *identifier, int value);
void free_ast(ast_node *node);

#endif // PARSER_H
