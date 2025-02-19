#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bytecode.h"
#include "vm.h"

#define STACK_SIZE 256

// Global stacks
int stack[STACK_SIZE];   // Integer stack
float fstack[STACK_SIZE]; // Floating-point stack
int sp = -1, fsp = -1;   // Stack pointers

void execute_bytecode() {
    Instruction *code = get_bytecode();
    int ip = 0;
    
    float fstack[STACK_SIZE];
    int stack[STACK_SIZE];
    int sp = -1, fsp = -1;

    while (ip < get_bytecode_size()) {
        Instruction instr = code[ip++];

        switch (instr.op) {
            case OP_PUSH_NUMBER:
                stack[++sp] = instr.operand;
                break;

            case OP_PUSH_FLOAT: {
                float fval;
                memcpy(&fval, &instr.operand, sizeof(float));
                fstack[++fsp] = fval;
                break;
            }

            case OP_ADD:
                stack[sp - 1] += stack[sp];
                sp--;
                break;

            case OP_FADD:
                fstack[fsp - 1] += fstack[fsp];
                fsp--;
                break;

            case OP_SUB:
                stack[sp - 1] -= stack[sp];
                sp--;
                break;

            case OP_FSUB:
                fstack[fsp - 1] -= fstack[fsp];
                fsp--;
                break;

            case OP_MUL:
                stack[sp - 1] *= stack[sp];
                sp--;
                break;

            case OP_FMUL:
                fstack[fsp - 1] *= fstack[fsp];
                fsp--;
                break;

            case OP_DIV:
                if (sp >= 1) {
                    float a = stack[sp - 1];
                    float b = stack[sp];
                    if (b == 0) {
                        printf("Error: Division by zero in OP_DIV\n");
                    } else {
                        float result = a / b;
                        fstack[++fsp] = result; // Store result in float stack
                        sp -= 2; // Remove both int values
                    }
                } else {
                    printf("Stack underflow in OP_DIV\n");
                }
                break;

                case OP_FDIV:
                printf("Before OP_FDIV: fsp=%d\n", fsp);
                if (fsp >= 1) {
                    if (fstack[fsp] == 0.0f) {
                        printf("Error: Division by zero in OP_FDIV\n");
                    } else {
                        fstack[fsp - 1] /= fstack[fsp];
                        fsp--;
                    }
                } else {
                    printf("Float stack underflow in OP_FDIV\n");
                }
                printf("After OP_FDIV: fsp=%d\n", fsp);
                break;
                
            case OP_PRINT:
                if (fsp >= 0) {
                    printf("Result: %f\n", fstack[fsp--]); // Print float result
                } else if (sp >= 0) {
                    printf("Result: %d\n", stack[sp--]); // Print integer result
                } else {
                    printf("Error: Nothing to print, stack is empty\n");
                }
                break;

            case OP_HALT:
                return;

            default:
                printf("Unknown opcode: %d\n", instr.op);
                break;
        }
    }
}

