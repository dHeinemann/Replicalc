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

int main() {
    double result;
    char expr[EXPR_LEN];
    while (1) {
        printf("> ");
        fgets(expr, EXPR_LEN, stdin);
        if (strcmp(trim(expr), "exit") == 0) { break; }
        if (strcmp(trim(expr), "quit") == 0) { break; }

        result = calculate(expr);
        printf("%f\n", result);
    }
    return 0;
}