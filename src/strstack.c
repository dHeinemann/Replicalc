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

#include <stdlib.h>

#include "strstack.h"

struct String_Stack* string_stack_new(int capacity, int length) {
    struct String_Stack* string_stack = (struct String_Stack*) malloc(sizeof(struct String_Stack));
    string_stack->capacity = capacity;
    string_stack->top = -1;
    string_stack->array = (char**) malloc(sizeof(char[capacity][length]));

    return string_stack;
}

void string_stack_free(struct String_Stack* string_stack) {
    free(string_stack->array);
    free(string_stack);
}

int string_stack_isFull(struct String_Stack* string_stack) {
    return string_stack->top == string_stack->capacity -1;
}

int string_stack_isEmpty(struct String_Stack* string_stack) {
    return string_stack->top == -1;
}

void string_stack_push(struct String_Stack* string_stack, char* item) {
    if (string_stack_isFull(string_stack)) {
        return;
    }

    string_stack->array[++string_stack->top] = item;
}

char* string_stack_pop(struct String_Stack* string_stack) {
    if (string_stack_isEmpty(string_stack)) {
        return NULL;
    }

    return string_stack->array[string_stack->top--];
}

char* string_stack_peek(struct String_Stack* string_stack) {
    if (string_stack_isEmpty(string_stack)) {
        return NULL;
    }

    return string_stack->array[string_stack->top];
}
