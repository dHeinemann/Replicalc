/*
 * Replicalc, a REPL-based calculator for programmers
 * Copyright (C) 2022 David Heinemann
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

package chartypes

// Test whether the given character is numeric.
func IsNumeric(c byte) bool {
	return (c >= 48 && c <= 57) /* 0-9 */ ||
		c == '.' ||
		c == ','
}

// Test whether the given character is a letter.
func IsLetter(c byte) bool {
	return (c >= 65 && c <= 90) ||
		(c >= 97 && c <= 122)
}

// Test whether the given character is a symbol.
func IsSymbol(c byte) bool {
	return c == '^' ||
		c == '/' ||
		c == '*' ||
		c == '+' ||
		c == '-' ||
		c == '%' ||
		c == '(' ||
		c == ')'
}
