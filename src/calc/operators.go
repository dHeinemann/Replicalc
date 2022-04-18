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

package calc

import "math"

// Indicates an operator's associativity.
type associativity byte

const (
	associativityLeft  associativity = iota
	associativityRight associativity = iota
)

// Represents an operator.
type operator struct {
	token         string
	precedence    int16
	associativity associativity
	evaluate      func(float64, float64) float64
}

// Represents an unknown/unsupported operator.
var invalidOperator = operator{}

// List of supported math operators.
var operators = [...]operator{
	{
		// todo: Convert this into a map for direct access by token
		token:         "^",
		precedence:    10,
		associativity: associativityRight,
		evaluate: func(a, b float64) float64 {
			return math.Pow(a, b)
		},
	},
	{
		token:         "/",
		precedence:    5,
		associativity: associativityLeft,
		evaluate: func(a, b float64) float64 {
			return a / b
		},
	}, {
		token:         "*",
		precedence:    5,
		associativity: associativityLeft,
		evaluate: func(a, b float64) float64 {
			return a * b
		},
	},
	{
		token:         "+",
		precedence:    0,
		associativity: associativityLeft,
		evaluate: func(a, b float64) float64 {
			return a + b
		},
	},
	{
		token:         "-",
		precedence:    0,
		associativity: associativityLeft,
		evaluate: func(a, b float64) float64 {
			return a - b
		},
	},
	invalidOperator,
}

// Get the operator corresponding to the given string. Returns invalidOperator if no matching operator is found.
func getOperator(s string) operator {
	for i := 0; i < len(operators); i++ {
		if operators[i].token == s {
			return operators[i]
		}
	}

	return invalidOperator
}

// Test whether a string is a math operator.
func isOperator(token string) bool {
	for i := 0; i < len(operators); i++ {
		if operators[i].token == token {
			return true
		}
	}

	return false
}
