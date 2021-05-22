#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "dstack.h"
#include "strstack.h"

#define TOKEN_LEN 20 /* Maximum permitted token length*/

/* Operator associativity */
#define ASSOC_LEFT 0
#define ASSOC_RIGHT 0

/* Token types */
#define TYPE_DIGIT 0
#define TYPE_OP 1
#define TYPE_ALPHA 2

/*
 * Test whether a string is a math operator.
 */
int is_op(char* token) {
    if (!token)
        return 0;

    return strcmp(token, "^") == 0
        || strcmp(token, "/") == 0
        || strcmp(token, "*") == 0
        || strcmp(token, "+") == 0
        || strcmp(token, "-") == 0;
}

/*
 * Get a token's precedence.
 */
int prec(char* token) {
    if (strcmp(token, "^") == 0) { return 10; }
    if (strcmp(token, "/") == 0) { return  5; }
    if (strcmp(token, "*") == 0) { return  5; }
    if (strcmp(token, "+") == 0) { return  0; }
    if (strcmp(token, "-") == 0) { return  0; }
    return -1;
}

/*
 * Get a token's associativity.
 */
int assoc(char* token) {
    if (strcmp(token, "^") == 0) {
        return ASSOC_RIGHT;
    }

    return ASSOC_LEFT;
}

/*
 * Test whether a character is numeric.
 */
int is_numeric(char c) {
    return (c >= 48 && c <= 57) /* 0-9 */
        || c == '.'
        || c == ',';
}

/*
 * Test whether a character is a letter.
 */
int is_alpha(char c) {
    return (c >= 65 && c <= 90)
        || (c >= 97 && c <= 122);
}

/*
 * Test whether a character is a symbol.
 */
int is_symbol(char c) {
    return c == '^'
        || c == '/'
        || c == '*'
        || c == '+'
        || c == '-'
        || c == '('
        || c == ')';
}

/*
 * Convert an expression into a series of tokens.
 */
int tokenize(char expr[], char** tokens) {
    int i;
    int element = 0;
    int index = 0;
    int lastType = -1;
    int empty;

    for (i = 0; (size_t) i < strlen(expr); i++) {
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

    return index + 1;
}

/*
 * Convert an infix expression to postfix.
 */
int infixToPostfix(char** tokens, int length, char** output) {
    int i;
    char* op1;
    char* op2;
    struct String_Stack* ops = string_stack_new(length, TOKEN_LEN);
    int outputIndex = 0;

    for (i = 0; i < length; i++) {
        if (is_op(tokens[i])) {
            op1 = tokens[i];
            while (is_op(string_stack_peek(ops))) {
                op2 = string_stack_peek(ops);
                if ((assoc(op1) == ASSOC_LEFT && prec(op1) <= prec(op2))
                    || (assoc(op1) == ASSOC_RIGHT && prec(op1) < prec(op2))) {
                        output[outputIndex++] = string_stack_pop(ops);
                } else {
                    break;
                }
            }
            string_stack_push(ops, op1);
        } else if (strcmp(tokens[i], "(") == 0) {
            string_stack_push(ops, tokens[i]);
        } else if (strcmp(tokens[i], ")") == 0) {
            while (strcmp(string_stack_peek(ops), "(") != 0) {
                output[outputIndex++] = string_stack_pop(ops);
            }
            string_stack_pop(ops); /* Discard left paren */
        } else {
            output[outputIndex++] = tokens[i];
        }
    }

    while (!string_stack_isEmpty(ops)) {
        output[outputIndex++] = string_stack_pop(ops);
    }
    
    string_stack_free(ops);
    return outputIndex;
}

/*
 * Evaluate a postfix expression and return the result.
 */
double evaluate(char** tokens, int numTokens) {
    int i;
    double result = 0;

    struct Double_Stack* stack = double_stack_new(numTokens, TOKEN_LEN);
    for (i = 0; i < numTokens; i++) {
        if (is_op(tokens[i])) {
            double op2 = double_stack_pop(stack);
            double op1 = double_stack_pop(stack);
            if (strcmp(tokens[i], "*") == 0) { double_stack_push(stack, (op1 * op2)); }
            if (strcmp(tokens[i], "/") == 0) { double_stack_push(stack, (op1 / op2)); }
            if (strcmp(tokens[i], "+") == 0) { double_stack_push(stack, (op1 + op2)); }
            if (strcmp(tokens[i], "-") == 0) { double_stack_push(stack, (op1 - op2)); }
            if (strcmp(tokens[i], "^") == 0) { double_stack_push(stack, pow(op1, op2)); }
        } else {
            double_stack_push(stack, strtod(tokens[i], NULL));
        }
    }

    result = double_stack_pop(stack);
    double_stack_free(stack);
    return result;
}

/*
 * Calculate the result of an expression.
 */
double calculate(char* expr) {
    int i;
    int numInfixTokens;

    char** infixTokens = (char**) malloc(sizeof(char[strlen(expr)][TOKEN_LEN]));
    for (i = 0; (size_t) i < strlen(expr); i++) {
        infixTokens[i] = (char*) malloc(sizeof(char[TOKEN_LEN]));
    }
    numInfixTokens = tokenize(expr, infixTokens);

    char** postfixTokens = (char**) malloc(sizeof(char[numInfixTokens][TOKEN_LEN]));
    for (i = 0; (size_t) i < strlen(expr); i++) {
        postfixTokens[i] = (char*) malloc(sizeof(char[TOKEN_LEN]));
    }
    int numPostfixTokens = infixToPostfix(infixTokens, numInfixTokens, postfixTokens);
    int result = evaluate(postfixTokens, numPostfixTokens);

    free(infixTokens);
    free(postfixTokens);
    return result;
}