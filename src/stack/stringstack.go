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

package stack

type StringStack struct {
	array []string
}

// Indicates whether the stack is empty.
func (ss *StringStack) IsEmpty() bool {
	return len((*ss).array) == 0
}

// Push a string onto the stack.
func (ss *StringStack) Push(s string) {
	(*ss).array = append((*ss).array, s)
}

// Pop a string from the stack.
func (ss *StringStack) Pop() (result string, ok bool) {
	if (*ss).IsEmpty() {
		return "", false
	}

	top := len((*ss).array) - 1
	topValue := (*ss).array[top]
	if top == 0 {
		(*ss).array = []string{}
	} else {
		(*ss).array = (*ss).array[:top]
	}

	return topValue, true
}

// Get the top-most element from the StringStack
func (ss *StringStack) Peek() (result string, ok bool) {
	if (*ss).IsEmpty() {
		return "", false
	}

	return (*ss).array[len((*ss).array)-1], true
}
