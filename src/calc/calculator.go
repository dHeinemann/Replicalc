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

import (
	"strconv"

	"dheinemann.com/replicalc/chartypes"
	"dheinemann.com/replicalc/stack"
)

// Token types
type tokenType byte

const (
	tokenTypeNone     tokenType = iota
	tokenTypeDigit    tokenType = iota
	tokenTypeOperator tokenType = iota
	tokenTypeLetter   tokenType = iota
)

// Error codes
type ErrorCode byte

const (
	ErrorSuccess               ErrorCode = iota
	ErrorUnbalancedParantheses ErrorCode = iota
	ErrorDivideByZero          ErrorCode = iota
	ErrorUnknownVariable       ErrorCode = iota
)

// Check whether the character at index i is the negative sign for the number that follows it.
func isNegative(expr string, i int) bool {
	if expr[i] != '-' {
		return false
	}

	nextCharIsNum := i < len(expr) && chartypes.IsNumeric(byte(expr[i+1]))
	if i == 0 && nextCharIsNum {
		return true
	}

	lastCharIsSymbol := chartypes.IsSymbol(expr[i-1])
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

		if chartypes.IsNumeric(expr[i]) || isNegative(expr, i) {
			if hasUnfinishedToken && lastTokenType != tokenTypeDigit {
				tokens = append(tokens, currentToken)
				currentToken = ""
			}

			currentToken += string(expr[i])
			lastTokenType = tokenTypeDigit
		} else if chartypes.IsSymbol(expr[i]) {
			if hasUnfinishedToken {
				tokens = append(tokens, currentToken)
				currentToken = ""
			}

			currentToken += string(expr[i])
			lastTokenType = tokenTypeOperator
		} else if chartypes.IsLetter(expr[i]) {
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

func evaluate(tokens []string) (result float64, errorCode ErrorCode, context string) {
	valueStack := stack.FloatStack{}

	for i := 0; i < len(tokens); i++ {
		if isOperator(tokens[i]) {
			val2, _ := valueStack.Pop()
			val1, _ := valueStack.Pop()

			op := getOperator(tokens[i])
			evaluatedVal := op.evaluate(val1, val2)

			valueStack.Push(evaluatedVal)
		} else {
			value, err := strconv.ParseFloat(tokens[i], 64)

			if err != nil {
				if VarExists(tokens[i]) {
					value = GetVar(tokens[i])
				} else {
					return 0.0, ErrorUnknownVariable, tokens[i]
				}
			}

			valueStack.Push(value)
		}
	}

	nextOp, _ := valueStack.Pop()
	return nextOp, ErrorSuccess, ""
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

func Calculate(expr string) (result float64, errorCode ErrorCode, context string) {
	if !hasBalancedParantheses(expr) {
		return 0, ErrorUnbalancedParantheses, ""
	}

	if len(expr) == 0 {
		return 0, ErrorSuccess, ""
	}

	infixTokens := tokenize(expr)
	postfixTokens := infixToPostfix(infixTokens)
	result, errorCode, context = evaluate(postfixTokens)

	return result, errorCode, context
}
