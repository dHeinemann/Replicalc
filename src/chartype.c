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

int is_numeric(char c) {
    return (c >= 48 && c <= 57) /* 0-9 */
        || c == '.'
        || c == ',';
}

int is_alpha(char c) {
    return (c >= 65 && c <= 90)
        || (c >= 97 && c <= 122);
}

int is_symbol(char c) {
    return c == '^'
        || c == '/'
        || c == '*'
        || c == '+'
        || c == '-'
        || c == '('
        || c == ')';
}
