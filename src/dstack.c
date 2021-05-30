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

#include "limits.h"
#include "dstack.h"

struct Double_Stack* double_stack_new() {
    struct Double_Stack* double_stack = (struct Double_Stack*) malloc(sizeof(struct Double_Stack));
    double_stack->capacity = STACK_MAX_CAP;
    double_stack->top = -1;
    double_stack->array = (double*) malloc(sizeof(double[STACK_MAX_CAP][EXPR_MAX_WIDTH]));

    return double_stack;
}

void double_stack_free(struct Double_Stack* double_stack) {
    free(double_stack->array);
    free(double_stack);
}

int double_stack_isFull(struct Double_Stack* double_stack) {
    return double_stack->top == double_stack->capacity -1;
}

int double_stack_isEmpty(struct Double_Stack* double_stack) {
    return double_stack->top == -1;
}

void double_stack_push(struct Double_Stack* double_stack, double item) {
    if (double_stack_isFull(double_stack)) {
        return;
    }

    double_stack->array[++double_stack->top] = item;
}

double double_stack_pop(struct Double_Stack* double_stack) {
    if (double_stack_isEmpty(double_stack)) {
        return -1; /* todo */
    }

    return double_stack->array[double_stack->top--];
}

double double_stack_peek(struct Double_Stack* double_stack) {
    if (double_stack_isEmpty(double_stack)) {
        return -1; /* todo */
    }

    return double_stack->array[double_stack->top];
}
