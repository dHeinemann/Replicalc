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

#ifndef STRFUN_H
#define STRFUN_H

/*
 * Trim whitespace surrounding a string.
 */
char* trim(char* str);

/*
 * Trim trailing zeros and period from decimal numbers.
 */
void format(double value, char* output);

/*
 * Detect exit attempt.
 */
int isQuit(char* expr);

#endif
