#include "ast.h"

#define MAX_IR_INSTRUCTIONS 100

IRInstruction ir_code[MAX_IR_INSTRUCTIONS];
int ir_count = 0;
int temp_count = 0;

typedef enum {
    PLUS,
    MINUS,
    ASTERISK,
    SLASH
} OperatorType;

// Create a new number node
ASTNode *new_number_node(int value, int is_float) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER;
    node->data.number_value = value;
    node->is_float = is_float;
    return node;
}

ASTNode *new_float_node(float value, int is_float) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_FLOAT;
    node->data.float_value = value;
    node->is_float = 1;
    return node;
}

// Create a new identifier node
ASTNode *new_identifier_node(char *name) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_IDENTIFIER;
    node->data.identifier = strdup(name);
    return node;
}

// Create a new binary operation node
ASTNode *new_binary_op_node(ASTNode *left, ASTNode *right, int operator, int is_float) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_OPERATOR;
    node->data.op.left = left;
    node->data.op.right = right;
    node->data.op.operator = operator;
    node->is_float = is_float;
    return node;
}

// Create a new assignment node (for variable assignments)
ASTNode *new_assignment_node(char *name, ASTNode *value) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    node->type = NODE_ASSIGNMENT;
    node->data.assignment.name = strdup(name);
    node->data.assignment.value = value;
    return node;
}

// Print AST (for debugging)
void print_ast(ASTNode *node) {
    if (!node) return;
    switch (node->type) {
        case NODE_NUMBER:
            printf("Number: %d\n", node->data.number_value);
            break;
        case NODE_IDENTIFIER:
            printf("Identifier: %s\n", node->data.identifier);
            break;
        case NODE_OPERATOR:
            printf("Binary Operation: %c\n", node->data.op.operator);
            print_ast(node->data.op.left);
            print_ast(node->data.op.right);
            break;
    }
}

// Free AST memory
void free_ast(ASTNode *node) {
    if (!node) return;
    if (node->type == NODE_IDENTIFIER) {
        free(node->data.identifier);
    } else if (node->type == NODE_OPERATOR) {
        free_ast(node->data.op.left);
        free_ast(node->data.op.right);
    }
    free(node);
}

float evaluate_ast(ASTNode *node) {
    if (!node) return 0;

    switch (node->type) {
        case NODE_NUMBER:
            return node->data.number_value;

        case NODE_IDENTIFIER:
            printf("Error: Cannot evaluate identifier %s\n", node->data.identifier);
            return 0;

        case NODE_OPERATOR:
            {
                int left_val = evaluate_ast(node->data.op.left);
                int right_val = evaluate_ast(node->data.op.right);
                switch (node->data.op.operator) {
                    case '+': return left_val + right_val;
                    case '-': return left_val - right_val;
                    case '*': return left_val * right_val;
                    case '/': 
                        if (right_val == 0.0f) {
                            printf("Error: Division by zero\n");
                            return 0.0f;
                        }
                        return node->is_float?(float) left_val/ right_val: left_val / right_val;
                    default:
                        printf("Error: Unknown operator %c\n", node->data.op.operator);
                        return 0;
                }
            }
    }
    return 0;
}

// Generate IR for AST
int generate_ir(ASTNode *node) {
    if (!node) return -1;

    if (node->type == NODE_NUMBER) {
        int temp = temp_count++;
        if(node->is_float){
            ir_code[ir_count++] = (IRInstruction){IR_CONST, temp, -1, -1, node->data.float_value, 1};   
        }
        else{
            ir_code[ir_count++] = (IRInstruction){IR_CONST, temp, node->data.number_value, -1, 0.0, 0};
        }
        return temp;
    }

    if (node->type == NODE_OPERATOR) {
        int left = generate_ir(node->data.op.left);
        int right = generate_ir(node->data.op.right);
        int temp = temp_count++;

        IR_OpType ir_op;
        switch (node->data.op.operator) {
            case '+': ir_op = node->is_float ? IR_FADD : IR_ADD; break;
            case '-': ir_op = node->is_float ? IR_FSUB : IR_SUB; break;
            case '*': ir_op = node->is_float ? IR_FMUL : IR_MUL; break;
            case '/': ir_op = node->is_float ? IR_FDIV : IR_DIV; break;
            default: printf("Unknown operator!\n"); exit(1);
        }

        ir_code[ir_count++] = (IRInstruction){ir_op, temp, left, right, 0.0, node->is_float};
        return temp;
    }

    return -1;
}

// Print the generated IR
void print_ir() {
    for (int i = 0; i < ir_count; i++) {
        printf("t%d = ", ir_code[i].result);
        if (ir_code[i].op == IR_CONST) {
            printf("%d\n", ir_code[i].left);
        } else {
            printf("t%d ", ir_code[i].left);
            switch (ir_code[i].op) {
                case IR_ADD: printf("+"); break;
                case IR_SUB: printf("-"); break;
                case IR_MUL: printf("*"); break;
                case IR_DIV: printf("/"); break;
            }
            printf(" t%d\n", ir_code[i].right);
        }
    }
}
