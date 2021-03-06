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
	"errors"
	"fmt"
	"strconv"

	"dheinemann.com/replicalc/chartypes"
	"dheinemann.com/replicalc/stack"
)

type Calculator struct {
	Variables func() VariableDb
}

// Token types
type tokenType byte

const (
	tokenTypeNone     tokenType = iota
	tokenTypeDigit    tokenType = iota
	tokenTypeOperator tokenType = iota
	tokenTypeLetter   tokenType = iota
)

type unknownVariableError struct {
	msg      string
	variable string
}

func (e unknownVariableError) Error() string {
	return e.msg
}

func unknownVariable(varName string) error {
	return unknownVariableError{
		msg:      fmt.Sprintf("Unknown variable: '%v'", varName),
		variable: varName,
	}
}

var ErrUnbalancedParantheses = errors.New("Unbalanced parentheses")

// Create and initialize a new Calculator.
func NewCalculator() Calculator {
	db := newVariableDb()
	return Calculator{
		Variables: func() VariableDb {
			return db
		},
	}
}

// Check whether the character at index i is the negative sign for the number that follows it.
func (calc Calculator) isNegativeSign(expr string, i int) bool {
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
func (calc Calculator) tokenize(expr string) ([]string, error) {
	lastTokenType := tokenTypeNone
	tokens := []string{}

	currentToken := ""
	for i := 0; i < len(expr); i++ {
		hasUnfinishedToken := len(currentToken) > 0

		if chartypes.IsNumeric(expr[i]) || calc.isNegativeSign(expr, i) {
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
		} else if expr[i] != ' ' {
			return nil, errors.New(fmt.Sprintf("Unknown character '%s' at index %v", string(expr[i]), i))
		}
	}

	if len(currentToken) > 0 {
		tokens = append(tokens, currentToken)
	}

	return tokens, nil
}

func (calc Calculator) infixToPostfix(tokens []string) []string {
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

// Evaluate an RPN expression
func (calc Calculator) evaluate(tokens []string) (float64, error) {
	valueStack := stack.FloatStack{}

	for i := 0; i < len(tokens); i++ {
		if isOperator(tokens[i]) {
			val2, _ := valueStack.Pop()
			val1, _ := valueStack.Pop()

			op := getOperator(tokens[i])
			if evaluatedVal, err := op.evaluate(val1, val2); err != nil {
				return 0, err
			} else {
				valueStack.Push(evaluatedVal)
			}
		} else {
			if value, err := strconv.ParseFloat(tokens[i], 64); err != nil {
				if calc.Variables().Exists(tokens[i]) {
					value = calc.Variables().Get(tokens[i])
					valueStack.Push(value)
				} else {
					return 0.0, unknownVariable(tokens[i])
				}
			} else {
				valueStack.Push(value)
			}
		}
	}

	nextOp, _ := valueStack.Pop()
	return nextOp, nil
}

func (calc Calculator) hasBalancedParantheses(expr string) bool {
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

// Evaluate an expression and return the result.
func (calc Calculator) Evaluate(expr string) (result float64, err error) {
	if !calc.hasBalancedParantheses(expr) {
		return 0, ErrUnbalancedParantheses
	}

	if len(expr) == 0 {
		return 0, nil
	}

	if infixTokens, err := calc.tokenize(expr); err != nil {
		return 0, err
	} else {
		postfixTokens := calc.infixToPostfix(infixTokens)
		result, err = calc.evaluate(postfixTokens)

		return result, err
	}
}
