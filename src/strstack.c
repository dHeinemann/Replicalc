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
#include "strstack.h"

struct StringStack* createStringStack()
{
    struct StringStack* StringStack = (struct StringStack*) malloc(sizeof(struct StringStack));
    StringStack->capacity = STACK_MAX_CAP;
    StringStack->top = -1;
    StringStack->array = (char**) malloc(sizeof(char[STACK_MAX_CAP]));

    return StringStack;
}

void freeStringStack(struct StringStack* stringStack)
{
    free(stringStack->array);
    free(stringStack);
}

int stringStackIsFull(struct StringStack* stringStack)
{
    return stringStack->top == stringStack->capacity -1;
}

int stringStackIsEmpty(struct StringStack* stringStack)
{
    return stringStack->top == -1;
}

void pushStringStack(struct StringStack* stringStack, char* item)
{
    if (stringStackIsFull(stringStack))
    {
        return;
    }

    stringStack->array[++stringStack->top] = item;
}

char* popStringStack(struct StringStack* stringStack)
{
    if (stringStackIsEmpty(stringStack))
    {
        return NULL;
    }

    return stringStack->array[stringStack->top--];
}

char* peekStringStack(struct StringStack* stringStack)
{
    if (stringStackIsEmpty(stringStack))
    {
        return NULL;
    }

    return stringStack->array[stringStack->top];
}
