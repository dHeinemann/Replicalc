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

#ifndef STRSTACK_HEADER
#define STRSTACK_HEADER

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
struct String_Stack* string_stack_new(int capacity, int length);

/*
 * De-allocate memory for a string_stack.
 */
void string_stack_free(struct String_Stack* string_stack);

/*
 * Test whether a string_stack is full.
 */
int string_stack_isFull(struct String_Stack* string_stack);

/*
 * Test whether a string_stack is empty.
 */
int string_stack_isEmpty(struct String_Stack* string_stack);

/*
 * Push a string onto a string_stack.
 */
void string_stack_push(struct String_Stack* string_stack, char* item);

/*
 * Remove the top-most element from the string_stack.
 */
char* string_stack_pop(struct String_Stack* string_stack);

/*
 * Get the top-most element from the string_stack.
 */
char* string_stack_peek(struct String_Stack* string_stack);

#endif
