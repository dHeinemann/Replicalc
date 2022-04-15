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

package calculator

import (
	"math"
	"strconv"

	"dheinemann.com/replicalc/chartype"
	"dheinemann.com/replicalc/stack"
)

// Operators
type operator struct {
	token         string
	precedence    byte
	associativity associativity
}

func getOperator(s string) operator {
	for i := 0; i < len(operators); i++ {
		if operators[i].token == s {
			return operators[i]
		}
	}

	return invalidOperator
}

var invalidOperator = operator{}
var operators = [...]operator{
	{"^", 10, associativityRight},
	{"/", 5, associativityLeft},
	{"*", 5, associativityLeft},
	{"+", 0, associativityLeft},
	{"-", 0, associativityLeft},
	invalidOperator,
}

// Operator associativity
type associativity byte

const (
	associativityLeft  associativity = iota
	associativityRight associativity = iota
)

// Token types
type tokenType byte

const (
	tokenTypeNone     tokenType = iota
	tokenTypeDigit    tokenType = iota
	tokenTypeOperator tokenType = iota
	tokenTypeLetter   tokenType = iota
)

// Test whether a string is a math operator.
func isOperator(token string) bool {
	for i := 0; i < len(operators); i++ {
		if operators[i].token == token {
			return true
		}
	}

	return false
}

// Error codes
type ErrorCode byte

const (
	ErrorSuccess               ErrorCode = iota
	ErrorUnbalancedParantheses ErrorCode = iota
	ErrorDivideByZero          ErrorCode = iota
)

// Check whether the character at index i is the negative sign for the number that follows it.
func isNegative(expr string, i int) bool {
	if expr[i] != '-' {
		return false
	}

	nextCharIsNum := i < len(expr) && chartype.IsNumeric(byte(expr[i+1]))
	if i == 0 && nextCharIsNum {
		return true
	}

	lastCharIsSymbol := chartype.IsSymbol(expr[i-1])
	lastCharIsSpace := expr[i-1] == ' '
	if nextCharIsNum && (lastCharIsSymbol || lastCharIsSpace) {
		return true
	}

	return false
}

// Convert an expression into a series of tokens.
func tokenize(expr string) []string {
	lastTokenType := tokenTypeNone
	tokens := []string{}

	currentToken := ""
	for i := 0; i < len(expr); i++ {
		hasUnfinishedToken := len(currentToken) > 0

		if chartype.IsNumeric(expr[i]) || isNegative(expr, i) {
			if hasUnfinishedToken && lastTokenType != tokenTypeDigit {
				tokens = append(tokens, currentToken)
				currentToken = ""
			}

			currentToken += string(expr[i])
			lastTokenType = tokenTypeDigit
		} else if chartype.IsSymbol(expr[i]) {
			if hasUnfinishedToken {
				tokens = append(tokens, currentToken)
				currentToken = ""
			}

			currentToken += string(expr[i])
			lastTokenType = tokenTypeOperator
		} else if chartype.IsLetter(expr[i]) {
			if hasUnfinishedToken && lastTokenType != tokenTypeLetter {
				tokens = append(tokens, currentToken)
				currentToken = ""
			}

			currentToken += string(expr[i])
			lastTokenType = tokenTypeLetter
		}
	}

	if len(currentToken) > 0 {
		tokens = append(tokens, currentToken)
	}

	return tokens
}

func infixToPostfix(tokens []string) []string {
	output := []string{}

	ops := stack.StringStack{}
	for i := 0; i < len(tokens); i++ {
		if isOperator(tokens[i]) {
			op1 := getOperator(tokens[i])
			for !ops.IsEmpty() {
				nextOp, _ := ops.Peek()
				if !isOperator(nextOp) {
					break
				}

				op2 := getOperator(nextOp)
				if (op1.associativity == associativityLeft && op1.precedence <= op2.precedence) ||
					(op1.associativity == associativityRight && op1.precedence < op2.precedence) {
					newOp, _ := ops.Pop()
					output = append(output, newOp)
				} else {
					break
				}
			}
			ops.Push(op1.token)
		} else if tokens[i] == "(" {
			ops.Push(tokens[i])
		} else if tokens[i] == ")" {
			for {
				nextOp, ok := ops.Peek()
				if ok && nextOp != "(" {
					ops.Pop() // value is already in nextOp, don't need to store it again
					output = append(output, nextOp)
				} else {
					break
				}
			}
			ops.Pop() // Discard left parenthesis
		} else {
			output = append(output, tokens[i])
		}
	}

	for !ops.IsEmpty() {
		nextOp, _ := ops.Pop()
		output = append(output, nextOp)
	}

	return output
}

func evaluate(tokens []string) (result float64, errorCode ErrorCode) {
	stack := stack.FloatStack{}

	for i := 0; i < len(tokens); i++ {
		if isOperator(string(tokens[i])) {
			op2, _ := stack.Pop()
			op1, _ := stack.Pop()

			if tokens[i] == "/" {
				if op2 == 0 {
					return 0, ErrorDivideByZero
				}

				stack.Push(op1 / op2)
			}

			if tokens[i] == "*" {
				stack.Push(op1 * op2)
			}
			if tokens[i] == "+" {
				stack.Push(op1 + op2)
			}
			if tokens[i] == "-" {
				stack.Push(op1 - op2)
			}
			if tokens[i] == "^" {
				stack.Push(math.Pow(op1, op2))
			}
		} else {
			value, _ := strconv.ParseFloat(tokens[i], 64)
			stack.Push(value)
		}
	}

	nextOp, _ := stack.Pop()
	return nextOp, ErrorSuccess
}

func hasBalancedParantheses(expr string) bool {
	numParens := 0
	for i := 0; i < len(expr); i++ {
		if expr[i] == '(' {
			numParens++
		} else if expr[i] == ')' {
			if numParens > 0 {
				numParens--
			} else {
				return false
			}
		}
	}

	return numParens == 0
}

func Calculate(expr string) (result float64, errorCode ErrorCode) {
	if !hasBalancedParantheses(expr) {
		return 0, ErrorUnbalancedParantheses
	}

	if len(expr) == 0 {
		return 0, ErrorSuccess
	}

	infixTokens := tokenize(expr)
	postfixTokens := infixToPostfix(infixTokens)
	return evaluate(postfixTokens)
}
