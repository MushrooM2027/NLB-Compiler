#include "bytecode.h"
#include <stdio.h>
#include <stdlib.h>

Instruction bytecode[256];  // Example: max 256 instructions
int bytecode_size = 0;

void generate_bytecode(ASTNode *node) {
    if (!node) return;

    switch (node->type) {
        case NODE_NUMBER:
            bytecode[bytecode_size++] = (Instruction){OP_PUSH_NUMBER, node->data.number_value};
            break;

        case NODE_FLOAT:
            // Store the float as an integer (bitwise representation)
            {
                int float_as_int;
                memcpy(&float_as_int, &node->data.float_value, sizeof(float));
                bytecode[bytecode_size++] = (Instruction){OP_PUSH_FLOAT, float_as_int};
            }
            break;

        case NODE_OPERATOR:
            generate_bytecode(node->data.op.left);
            generate_bytecode(node->data.op.right);

            // Determine if we need float operations
            int is_float = (node->data.op.left->is_float || node->data.op.right->is_float);

            if (is_float) {
                if (!node->data.op.left->is_float) {
                    int float_as_int;
                    float float_val = (float)node->data.op.left->data.number_value;
                    memcpy(&float_as_int, &float_val, sizeof(float));
                    bytecode[bytecode_size++] = (Instruction){OP_PUSH_FLOAT, float_as_int};
                }
                if (!node->data.op.right->is_float) {
                    int float_as_int;
                    float float_val = (float)node->data.op.right->data.number_value;
                    memcpy(&float_as_int, &float_val, sizeof(float));
                    bytecode[bytecode_size++] = (Instruction){OP_PUSH_FLOAT, float_as_int};
                }
            }
            
            switch (node->data.op.operator) {
                case '+': bytecode[bytecode_size++] = is_float ? (Instruction){OP_FADD, 0} : (Instruction){OP_ADD, 0}; break;
                case '-': bytecode[bytecode_size++] = is_float ? (Instruction){OP_FSUB, 0} : (Instruction){OP_SUB, 0}; break;
                case '*': bytecode[bytecode_size++] = is_float ? (Instruction){OP_FMUL, 0} : (Instruction){OP_MUL, 0}; break;
                case '/': bytecode[bytecode_size++] = is_float ? (Instruction){OP_FDIV, 0} : (Instruction){OP_DIV, 0}; break;
            }
            break;
    }
}



void finalize_bytecode() {
    bytecode[bytecode_size++] = (Instruction){OP_PRINT, 0};
    bytecode[bytecode_size++] = (Instruction){OP_HALT, 0};
}


Instruction *get_bytecode() {
    return bytecode;
}

int get_bytecode_size() {
    return bytecode_size;
}

void print_bytecode() {
    printf("Printing bytecode...\n");
    for (int i = 0; i < bytecode_size; i++) {
        printf("Instruction %d: Opcode = %d, Operand = %d\n", i, bytecode[i].op, bytecode[i].operand);
    }
}
