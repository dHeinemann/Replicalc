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

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "calc.h"

#define EXPR_LEN 128 /* Maximum buffer size for expression input */

/*
 * Trim whitespace surrounding a string.
 */
char* trim(char* str) {
    int i;

    /* Trim start */
    for (i = 0; (size_t) i < strlen(str); i++) {
        if (!isspace(str[i])) {
            break;
        }
        str++;
    }

    /* Trim end */
    for (i = (int) strlen(str) - 1; i >= 0; i--) {
        if (!isspace(str[i])) {
            break;
        }
        str[i] = '\0';
    }

    return str;
}

/*
 * Trim trailing zeros and period from decimal numbers.
 */
void format(double value, char* output) {
    int i;

    sprintf(output, "%f", value);
    for (i = (int) strlen(output) - 1; i >= 0; i--) {
        if (output[i] == '0') {
            output[i] = '\0';
        } else {
            if (output[i] == '.') {
                output[i] = '\0';
            }
            break;
        }
    }
}

int main() {
    double result;
    char expr[EXPR_LEN];
    char resultStr[EXPR_LEN];
    while (1) {
        printf("> ");
        fgets(expr, EXPR_LEN, stdin);
        if (strcmp(trim(expr), "exit") == 0) { break; }
        if (strcmp(trim(expr), "quit") == 0) { break; }

        int errorCode = Error_Success;
        result = calculate(expr, &errorCode);
        switch (errorCode) {
            case Error_Success:
                format(result, resultStr);
                printf("%s\n", resultStr);
                break;
            case Error_UnbalParens:
                printf("Error: Unbalanced parentheses\n");
                break;
            case Error_DivideByZero:
                printf("Error: Division by zero\n");
                break;
        }
    }
    return 0;
}