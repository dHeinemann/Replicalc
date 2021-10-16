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
 * Represents a StringStack of strings.
 */
struct StringStack
{
    int top;
    int capacity;
    char** array;
};

/*
 * Create a new StringStack.
 */
struct StringStack* createStringStack();

/*
 * De-allocate memory for a StringStack.
 */
void freeStringStack(struct StringStack* stringStack);

/*
 * Test whether a StringStack is full.
 */
int stringStackIsFull(struct StringStack* stringStack);

/*
 * Test whether a StringStack is empty.
 */
int stringStackIsEmpty(struct StringStack* stringStack);

/*
 * Push a string onto a StringStack.
 */
void pushStringStack(struct StringStack* stringStack, char* item);

/*
 * Remove the top-most element from the StringStack.
 */
char* popStringStack(struct StringStack* stringStack);

/*
 * Get the top-most element from the StringStack.
 */
char* peekStringStack(struct StringStack* stringStack);

#endif
