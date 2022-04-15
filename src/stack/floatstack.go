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

type FloatStack struct {
	array []float64
}

// Indicates whether the stack is empty.
func (fs *FloatStack) IsEmpty() bool {
	return len((*fs).array) == 0
}

// Push a float64 onto the stack.
func (fs *FloatStack) Push(s float64) {
	(*fs).array = append((*fs).array, s)
}

// Pop a float64 from the stack.
func (fs *FloatStack) Pop() (result float64, ok bool) {
	if (*fs).IsEmpty() {
		return 0.0, false
	}

	top := len((*fs).array) - 1
	topValue := (*fs).array[top]
	if top == 0 {
		(*fs).array = []float64{}
	} else {
		(*fs).array = (*fs).array[:top]
	}

	return topValue, true
}

// Get the top-most element from the FloatStack
func (fs *FloatStack) Peek() (result float64, ok bool) {
	if (*fs).IsEmpty() {
		return 0.0, false
	}
	return (*fs).array[len((*fs).array)-1], true
}
