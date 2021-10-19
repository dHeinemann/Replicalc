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
#include <string.h>

#include "history.h"
#include "limits.h"
#include "strstack.h"
#include "strfun.h"

struct History* createHistory()
{
    struct History* history;

    history = malloc(sizeof(struct History));
    history->older = createStringStack();
    history->newer = createStringStack();

    history->current = malloc(sizeof(char) * EXPR_LEN);

    return history;
}

void destroyHistory(struct History* history)
{
    while (!stringStackIsEmpty(history->older))
        free(popStringStack(history->older));
    freeStringStack(history->older);

    while (!stringStackIsEmpty(history->newer))
        free(popStringStack(history->newer));
    freeStringStack(history->newer);

    free(history->current);

    free(history);
}

int previousExpression(struct History* history, char* buffer)
{
    if (stringStackIsEmpty(history->older))
        return 0;
    
    if (history->current != NULL)
        pushStringStack(history->newer, history->current);

    history->current = popStringStack(history->older);

    if (buffer != NULL)
        copyString(buffer, history->current, EXPR_LEN);

    return 1;
}

int nextExpression(struct History* history, char buffer[])
{
    if (stringStackIsEmpty(history->newer))
        return 0;

    /* todo: using this with the last expression produces garbled text on DOS */
    if (history->current != NULL)
        pushStringStack(history->older, history->current);

    history->current = popStringStack(history->newer);

    if (buffer != NULL)
        copyString(buffer, history->current, EXPR_LEN);

    return 1;
}

void resetHistory(struct History* history)
{
    while (!stringStackIsEmpty(history->newer))
    {
        nextExpression(history, NULL);
    }
}

void addToHistory(struct History* history, char expression[])
{
    int isSameAsLastExpression;
    char* exp;

    resetHistory(history);

    if (!stringStackIsEmpty(history->older))
    {
        isSameAsLastExpression = strncmp(expression, peekStringStack(history->older), EXPR_LEN) == 0;
        if (isSameAsLastExpression)
            return;
    }

    exp = malloc(sizeof(char) * EXPR_LEN);
    copyString(exp, expression, EXPR_LEN);
    pushStringStack(history->older, exp);
}