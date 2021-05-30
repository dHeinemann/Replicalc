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

#ifndef DSTACK_HEADER
#define DSTACK_HEADER

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
struct Double_Stack* double_stack_new();

/*
 * De-allocate memory for a double_stack.
 */
void double_stack_free(struct Double_Stack* double_stack);

/*
 * Test whether a double_stack is full.
 */
int double_stack_isFull(struct Double_Stack* double_stack);

/*
 * Test whether a double_stack is empty.
 */
int double_stack_isEmpty(struct Double_Stack* double_stack);

/*
 * Push a string onto a double_stack.
 */
void double_stack_push(struct Double_Stack* double_stack, double item);

/*
 * Remove the top-most element from the double_stack.
 */
double double_stack_pop(struct Double_Stack* double_stack);

/*
 * Get the top-most element from the double_stack.
 */
double double_stack_peek(struct Double_Stack* double_stack);

#endif
