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

#ifndef CHARTYPE_H
#define CHARTYPE_H

/*
 * Test whether a character is numeric.
 */
int is_numeric(char c);

/*
 * Test whether a character is a letter.
 */
int is_alpha(char c);

/*
 * Test whether a character is a symbol.
 */
int is_symbol(char c);

#endif