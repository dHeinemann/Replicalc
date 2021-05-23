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

#ifndef CALC_HEADER
#define CALC_HEADER

enum Error {
    /* No error */
    Error_Success,

    /* Parentheses are unbalanced */
    Error_UnbalParens,

    /* Division by zero */
    Error_DivideByZero
};

/*
 * Calculate the result of an expression.
 */
double calculate(char* expr, int* errorCode);

#endif
