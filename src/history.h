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

#ifndef HISTORY_H
#define HISTORY_H

/**
 * Represents the user's input history.
 */
struct History {
    /**
     * Input older than the current item.
     */
    struct StringStack* older;

    /**
     * Input newer than the current item.
     */
    struct StringStack* newer;

    /**
     * The current item.
     */
    char* current;
};

/**
 * Create a new History.
 */
struct History* createHistory();

/**
 * De-allocate a history, and its members.
 */
void destroyHistory(struct History* history);

/**
 * Get the previous history item.
 */
int previousExpression(struct History* history, char* buffer);

/**
 * Get the next history item.
 */
int nextExpression(struct History* history, char buffer[]);

/**
 * Reset history to the end (most recent item);
 */
void resetHistory(struct History* history);

/**
 * Add an expression to the history.
 */
void addToHistory(struct History* history, char expression[]);

#endif
