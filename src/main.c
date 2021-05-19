#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Array limits */
/*#define MAX_TOKENS 128*/
#define TOKEN_LEN 20

/* Operator associativity */
#define ASSOC_LEFT 0
#define ASSOC_RIGHT 0

/* Token types */
#define TYPE_DIGIT 0
#define TYPE_OP 1
#define TYPE_ALPHA 2

struct Stack {
    int top;
    unsigned capacity;
    char** array;
};

struct Stack* stack_new(unsigned capacity) {
    struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (char**) malloc(sizeof(char[capacity][TOKEN_LEN]));

    return stack;
}

void stack_free(struct Stack* stack) {
    free(stack->array);
    free(stack);
}

int stack_isFull(struct Stack* stack) {
    return stack->top == stack->capacity -1;
}

int stack_isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

void stack_push(struct Stack* stack, char* item) {
    if (stack_isFull(stack)) {
        return;
    }

    stack->array[++stack->top] = item;
}

char* stack_pop(struct Stack* stack) {
    if (stack_isEmpty(stack)) {
        return NULL;
    }

    return stack->array[stack->top--];
}

char* stack_peek(struct Stack* stack) {
    if (stack_isEmpty(stack)) {
        return NULL;
    }

    return stack->array[stack->top];
}

int is_op(char* token) {
    if (!token)
        return 0;

    return strcmp(token, "^") == 0
        || strcmp(token, "/") == 0
        || strcmp(token, "*") == 0
        || strcmp(token, "+") == 0
        || strcmp(token, "-") == 0;
}

int prec(char* token) {
    if (strcmp(token, "^") == 0) { return 10; }
    if (strcmp(token, "/") == 0) { return  5; }
    if (strcmp(token, "*") == 0) { return  5; }
    if (strcmp(token, "+") == 0) { return  0; }
    if (strcmp(token, "-") == 0) { return  0; }
    return -1;
}

int assoc(char* token) {
    if (strcmp(token, "^") == 0) {
        return ASSOC_RIGHT;
    }

    return ASSOC_LEFT;
}

int is_numeric(int c) {
    return (c >= 48 && c <= 57) /* 0-9 */
        || c == '.'
        || c == ',';
}

int is_alpha(int c) {
    return (c >= 65 && c <= 90)
        || (c >= 97 && c <= 122);
}

int is_symbol(int c) {
    return c == '^'
        || c == '/'
        || c == '*'
        || c == '+'
        || c == '-'
        || c == '('
        || c == ')';
}

int tokenize(char expr[], char** tokens) {
    /*char tokens[MAX_TOKENS][TOKEN_LEN];*/
    int i;
    int element = 0;
    int index = 0;
    int lastType = -1;
    int empty;

    for (i = 0; i < strlen(expr); i++) {
        empty = index == 0 && element == 0;
        if (is_numeric(expr[i])) {
            if (!empty && lastType != TYPE_DIGIT) {
                tokens[index++][element] = '\0';
                element = 0;
            }

            tokens[index][element++] = expr[i];
            lastType = TYPE_DIGIT;
        } else if (is_symbol(expr[i])) {
            if (!empty) {
                tokens[index++][element] = '\0';
                element = 0;
            }

            tokens[index][element++] = expr[i];
            lastType = TYPE_OP;
        } else if (is_alpha(expr[i])) {
            if (!empty && lastType != TYPE_ALPHA) {
                tokens[index++][element] = '\0';
                element = 0;
            }

            tokens[index][element++] = expr[i];
            lastType = TYPE_ALPHA;
        }
    }

    tokens[index][element] = '\0';
    tokens[++index][0] = '\0';

/*
    for (i = 0; i < index; i++) {
        printf("%s\n", tokens[i]);
    }
    */

    return index;
}

void infixToPostfix(char** tokens, int length, char** output) {
    int i;
    char* op1;
    char* op2;
    struct Stack* ops = stack_new(length);
    int outputIndex = 0;

    for (i = 0; i < length; i++) {
        if (is_op(tokens[i])) {
            op1 = tokens[i];
            while (is_op(stack_peek(ops))) {
                op2 = stack_peek(ops);
                if (assoc(op1) == ASSOC_LEFT && prec(op1) <= prec(op2)
                    || assoc(op1) == ASSOC_RIGHT && prec(op1) < prec(op2)) {
                        output[outputIndex++] = stack_pop(ops);
                } else {
                    break;
                }
            }
            stack_push(ops, op1);
        } else if (strcmp(tokens[i], "(") == 0) {
            stack_push(ops, tokens[i]);
        } else if (strcmp(tokens[i], ")") == 0) {
            while (strcmp(stack_peek(ops), "(") != 0) {
                output[outputIndex++] = stack_pop(ops);
            }
            stack_pop(ops); /* Discard left paren */
        } else {
            output[outputIndex++] = tokens[i];
        }
    }

    while (!stack_isEmpty(ops)) {
        output[outputIndex++] = stack_pop(ops);
    }

    for (i = 0; i < outputIndex; i++) {
        printf("%s ", output[i]);
    }
    
    stack_free(ops);
}

int main() {
    int i;
    char* expr = "4 + 4 * 2 / ( 1 - 5 )";
    int numTokens;

    char** infixTokens = (char**) malloc(sizeof(char[strlen(expr)][TOKEN_LEN]));
    for (i = 0; i < strlen(expr); i++) {
        infixTokens[i] = (char*) malloc(sizeof(char[TOKEN_LEN]));
    }
    numTokens = tokenize(expr, infixTokens);

    char** postfixTokens = (char**) malloc(sizeof(char[numTokens][TOKEN_LEN]));
    for (i = 0; i < strlen(expr); i++) {
        postfixTokens[i] = (char*) malloc(sizeof(char[TOKEN_LEN]));
    }
    infixToPostfix(infixTokens, numTokens, postfixTokens);

    free(infixTokens);
    free(postfixTokens);
    return 0;
}