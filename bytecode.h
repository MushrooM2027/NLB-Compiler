#ifndef BYTECODE_H
#define BYTECODE_H

#include "ast.h"
#include <stdint.h>

typedef enum {
    OP_PUSH_NUMBER, 
    OP_PUSH_FLOAT,
    OP_ADD, 
    OP_SUB, 
    OP_MUL, 
    OP_DIV,
    OP_FADD, 
    OP_FSUB, 
    OP_FMUL, 
    OP_FDIV,
    OP_PRINT, 
    OP_HALT,    // Stop execution
} OpCode;

typedef struct {
    OpCode op;
    int operand;
} Instruction;

// Declare bytecode storage (extern for global access)
extern Instruction bytecode[];
extern int bytecode_size;

// Function to generate bytecode
void generate_bytecode(ASTNode *node);
Instruction *get_bytecode();  // Declare function
int get_bytecode_size(); 
void print_bytecode();
void finalize_bytecode();

#endif
