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

struct DoubleStack* createDoubleStack()
{
    struct DoubleStack* doubleStack = (struct DoubleStack*) malloc(sizeof(struct DoubleStack));
    doubleStack->capacity = STACK_MAX_CAP;
    doubleStack->top = -1;
    doubleStack->array = (double*) malloc(sizeof(double[STACK_MAX_CAP]));

    return doubleStack;
}

void freeDoubleStack(struct DoubleStack* doubleStack)
{
    free(doubleStack->array);
    free(doubleStack);
}

int doubleStackIsFull(struct DoubleStack* doubleStack)
{
    return doubleStack->top == doubleStack->capacity -1;
}

int doubleStackIsEmpty(struct DoubleStack* doubleStack)
{
    return doubleStack->top == -1;
}

void pushDoubleStack(struct DoubleStack* doubleStack, double item)
{
    if (doubleStackIsFull(doubleStack))
    {
        return;
    }

    doubleStack->array[++doubleStack->top] = item;
}

double popDoubleStack(struct DoubleStack* doubleStack)
{
    if (doubleStackIsEmpty(doubleStack))
    {
        return -1; /* todo */
    }

    return doubleStack->array[doubleStack->top--];
}

double peekDoubleStack(struct DoubleStack* doubleStack)
{
    if (doubleStackIsEmpty(doubleStack))
    {
        return -1; /* todo */
    }

    return doubleStack->array[doubleStack->top];
}
