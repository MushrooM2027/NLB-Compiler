#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { NODE_NUMBER,NODE_FLOAT, NODE_IDENTIFIER, NODE_OPERATOR,NODE_ASSIGNMENT } NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        int number_value;  // For NUMBER nodes
        float float_value; // For FLOAT nodes
        char *identifier;  // For IDENTIFIER nodes
        struct {           // For OPERATOR nodes
            struct ASTNode *left;
            struct ASTNode *right;
            char operator;
            int is_float;
        } op;
        struct {
            char *name;
            struct ASTNode *value;
        } assignment;  
    } data;
    int is_float;
} ASTNode;

// IR Operation Types
typedef enum { IR_ADD, IR_SUB, IR_MUL, IR_DIV, IR_CONST,IR_FADD,IR_FSUB,IR_FMUL,IR_FDIV, } IR_OpType;

// IR Instruction Structure
typedef struct {
    IR_OpType op;
    int result;
    int left;
    int right;
    float float_result;  // New field for float computations
    int is_float; // To track if it's a floating-point operation
} IRInstruction;

// Function prototypes
ASTNode *new_number_node(int value, int is_float);
ASTNode *new_float_node(float value, int is_float);
ASTNode *new_identifier_node(char *name);
ASTNode *new_binary_op_node(ASTNode *left, ASTNode *right, int operator, int is_float);
ASTNode *new_assignment_node(char *name, ASTNode *value);
void print_ast(ASTNode *node);
void free_ast(ASTNode *node);

// IR Functions
int generate_ir(ASTNode *node);
void print_ir();

#endif
