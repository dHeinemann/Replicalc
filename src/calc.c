/*
 * Replicalc, a REPL-based calculator for programmers
 * Copyright (C) 2021 David Heinemann
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "calc.h"
#include "chartype.h"
#include "dstack.h"
#include "limits.h"
#include "strstack.h"

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
 * Check whether the character at i is the negative sign for the number that follows.
 */
int is_negative(char expr[], int i) {
    int nextCharIsNum; /* Negative sign is to the immediate left of the number that follows. */
    int lastCharIsSymbol;
    int lastCharIsSpace;
    if (expr[i] != '-') {
        return 0;
    }

    /* Expression begins with a negative number */
    nextCharIsNum = (size_t) i < strlen(expr) && is_numeric(expr[i+1]);
    if (i == 0 && nextCharIsNum) {
        return 1;
    }

    /* Negative sign belongs to a number */
    lastCharIsSymbol = is_symbol(expr[i-1]);
    lastCharIsSpace = expr[i-1] == ' ';
    if (nextCharIsNum && (lastCharIsSymbol || lastCharIsSpace)) {
        return 1;
    }

    return 0;
}

/*
 * Convert an expression into a series of tokens.
 */
int tokenize(char expr[], char** tokens) {
    int i;
    int element;
    int index;
    int lastType;
    int empty;

    element = 0;
    index = 0;
    lastType = -1;

    for (i = 0; (size_t) i < strlen(expr); i++) {
        empty = index == 0 && element == 0;
        if (is_numeric(expr[i]) || is_negative(expr, i)) {
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
    struct String_Stack* ops = string_stack_new();
    int outputIndex = 0;

    for (i = 0; i < length; i++) {
        if (is_op(tokens[i])) {
            op1 = tokens[i];
            while (is_op(string_stack_peek(ops))) {
                op2 = string_stack_peek(ops);
                if ((assoc(op1) == ASSOC_LEFT && prec(op1) <= prec(op2))
                    || (assoc(op1) == ASSOC_RIGHT && prec(op1) < prec(op2))) {
                        strcpy(output[outputIndex++], string_stack_pop(ops));
                } else {
                    break;
                }
            }
            string_stack_push(ops, op1);
        } else if (strcmp(tokens[i], "(") == 0) {
            string_stack_push(ops, tokens[i]);
        } else if (strcmp(tokens[i], ")") == 0) {
            while (strcmp(string_stack_peek(ops), "(") != 0) {
                strcpy(output[outputIndex++], string_stack_pop(ops));
            }
            string_stack_pop(ops); /* Discard left paren */
        } else {
            strcpy(output[outputIndex++], tokens[i]);
        }
    }

    while (!string_stack_isEmpty(ops)) {
        strcpy(output[outputIndex++], string_stack_pop(ops));
    }
    
    string_stack_free(ops);
    return outputIndex;
}

/*
 * Evaluate a postfix expression and return the result.
 */
double evaluate(char** tokens, int numTokens, int* errorCode) {
    int i;
    double result;

    struct Double_Stack* stack = double_stack_new();
    for (i = 0; i < numTokens; i++) {
        if (is_op(tokens[i])) {
            double op2 = double_stack_pop(stack);
            double op1 = double_stack_pop(stack);
            if (strcmp(tokens[i], "/") == 0) {
                if (op2 == 0) {
                    *errorCode = Error_DivideByZero;
                    double_stack_free(stack);
                    return 0;
                }
                double_stack_push(stack, (op1 / op2));
            }
            if (strcmp(tokens[i], "*") == 0) { double_stack_push(stack, (op1 * op2)); }
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
 * Check whether an expression has balanced parentheses.
 */
int hasBalancedParens(char* expr) {
    int i;
    int numParens = 0;
    for (i = 0; (size_t) i < strlen(expr); i++) {
        if (expr[i] == '(') {
            numParens++;
        } else if (expr[i] == ')') {
            if (numParens > 0) {
                numParens--;
            } else {
                return 0;
            }
        }
    }

    return numParens == 0;
}

double calculate(char* expr, int* errorCode) {
    double result;
    int i;

    char** infixtokens;
    int numinfixtokens;
    char** postfixtokens;
    int numpostfixtokens;

    if (!hasBalancedParens(expr)) {
        *errorCode = Error_UnbalParens;
        return 0;
    }

    infixtokens = (char**) malloc(sizeof(char[ARRAY_MAX_ELEM][TOKEN_LEN]));
    for (i = 0; (size_t) i < strlen(expr); i++) {
        infixtokens[i] = (char*) malloc(sizeof(char[TOKEN_LEN]));
    }
    numinfixtokens = tokenize(expr, infixtokens);

    postfixtokens = (char**) malloc(sizeof(char[ARRAY_MAX_ELEM][TOKEN_LEN]));
    for (i = 0; (size_t) i < strlen(expr); i++) {
        postfixtokens[i] = (char*) malloc(sizeof(char[TOKEN_LEN]));
    }
    numpostfixtokens = infixToPostfix(infixtokens, numinfixtokens, postfixtokens);
    result = evaluate(postfixtokens, numpostfixtokens, errorCode);

    for (i = 0; (size_t) i < strlen(expr); i++) {
        if (infixtokens[i]) {
            free(infixtokens[i]);
            free(postfixtokens[i]);
        }
    }
    free(infixtokens);
    free(postfixtokens);

    return result;
}
