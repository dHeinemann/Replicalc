#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TOKEN_LEN 20 /* Maximum permitted token length*/

/* Operator associativity */
#define ASSOC_LEFT 0
#define ASSOC_RIGHT 0

/* Token types */
#define TYPE_DIGIT 0
#define TYPE_OP 1
#define TYPE_ALPHA 2

/*
 * Represents a double_stack of strings.
 */
struct Double_Stack {
    int top;
    int capacity;
    double* array;
};

/*
 * Create a new double_stack.
 */
struct Double_Stack* double_stack_new(int capacity) {
    struct Double_Stack* double_stack = (struct Double_Stack*) malloc(sizeof(struct Double_Stack));
    double_stack->capacity = capacity;
    double_stack->top = -1;
    double_stack->array = (double*) malloc(sizeof(double[capacity][TOKEN_LEN]));

    return double_stack;
}

/*
 * De-allocate memory for a double_stack.
 */
void double_stack_free(struct Double_Stack* double_stack) {
    free(double_stack->array);
    free(double_stack);
}

/*
 * Test whether a double_stack is full.
 */
int double_stack_isFull(struct Double_Stack* double_stack) {
    return double_stack->top == double_stack->capacity -1;
}

/*
 * Test whether a double_stack is empty.
 */
int double_stack_isEmpty(struct Double_Stack* double_stack) {
    return double_stack->top == -1;
}

/*
 * Push a string onto a double_stack.
 */
void double_stack_push(struct Double_Stack* double_stack, double item) {
    if (double_stack_isFull(double_stack)) {
        return;
    }

    double_stack->array[++double_stack->top] = item;
}

/*
 * Remove the top-most element from the double_stack.
 */
double double_stack_pop(struct Double_Stack* double_stack) {
    if (double_stack_isEmpty(double_stack)) {
        return -1; /* todo */
    }

    return double_stack->array[double_stack->top--];
}

/*
 * Get the top-most element from the double_stack.
 */
double double_stack_peek(struct Double_Stack* double_stack) {
    if (double_stack_isEmpty(double_stack)) {
        return -1; /* todo */
    }

    return double_stack->array[double_stack->top];
}

/*
 * Represents a string_stack of strings.
 */
struct String_Stack {
    int top;
    int capacity;
    char** array;
};

/*
 * Create a new string_stack.
 */
struct String_Stack* string_stack_new(int capacity) {
    struct String_Stack* string_stack = (struct String_Stack*) malloc(sizeof(struct String_Stack));
    string_stack->capacity = capacity;
    string_stack->top = -1;
    string_stack->array = (char**) malloc(sizeof(char[capacity][TOKEN_LEN]));

    return string_stack;
}

/*
 * De-allocate memory for a string_stack.
 */
void string_stack_free(struct String_Stack* string_stack) {
    free(string_stack->array);
    free(string_stack);
}

/*
 * Test whether a string_stack is full.
 */
int string_stack_isFull(struct String_Stack* string_stack) {
    return string_stack->top == string_stack->capacity -1;
}

/*
 * Test whether a string_stack is empty.
 */
int string_stack_isEmpty(struct String_Stack* string_stack) {
    return string_stack->top == -1;
}

/*
 * Push a string onto a string_stack.
 */
void string_stack_push(struct String_Stack* string_stack, char* item) {
    if (string_stack_isFull(string_stack)) {
        return;
    }

    string_stack->array[++string_stack->top] = item;
}

/*
 * Remove the top-most element from the string_stack.
 */
char* string_stack_pop(struct String_Stack* string_stack) {
    if (string_stack_isEmpty(string_stack)) {
        return NULL;
    }

    return string_stack->array[string_stack->top--];
}

/*
 * Get the top-most element from the string_stack.
 */
char* string_stack_peek(struct String_Stack* string_stack) {
    if (string_stack_isEmpty(string_stack)) {
        return NULL;
    }

    return string_stack->array[string_stack->top];
}

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
double ldpow(double value, double exp) {
    double i;
    double result = 0;
    for (i = 0; i < exp; i++)
}
*/

/*
 * Convert an expression into a series of tokens.
 */
int tokenize(char expr[], char** tokens) {
    /*char tokens[MAX_TOKENS][TOKEN_LEN];*/
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
    tokens[++index][0] = '\0';

    /*
    for (i = 0; i < index; i++) {
        printf("%s\n", tokens[i]);
    }
    */

    return index;
}

/*
 * Convert an infix expression to postfix.
 */
int infixToPostfix(char** tokens, int length, char** output) {
    int i;
    char* op1;
    char* op2;
    struct String_Stack* ops = string_stack_new(length);
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

    /*
    for (i = 0; i < outputIndex; i++) {
        printf("%s ", output[i]);
    }
    */
    
    string_stack_free(ops);
    return outputIndex;
}

double evaluate(char** tokens, int numTokens) {
    int i;
    double result = 0;

    /*
        4 4 2 * 1 5 - / + 
    */
    struct Double_Stack* stack = double_stack_new(numTokens);
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

int main() {
    int i;
    char* expr = "4 + 4 * 2 / ( 1 - 5 )";
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

    printf("%d\n", result);

    free(infixTokens);
    free(postfixTokens);
    return 0;
}