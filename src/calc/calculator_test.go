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
	"testing"

	"github.com/stretchr/testify/assert"
)

var calc Calculator

func TestMain(m *testing.M) {
	calc = NewCalculator()
	m.Run()
}

func TestEvaluate_OnePlusOne_Returns2(t *testing.T) {
	result, _ := calc.Evaluate("1 + 1")
	assert.Equal(t, 2.0, result)
}

func TestEvaluate_UnbalancedOpenParen_ReturnsError(t *testing.T) {
	_, err := calc.Evaluate("3 * (1/4")
	assert.Equal(t, ErrUnbalancedParantheses, err)
}

func TestEvaluate_UnbalancedCloseParen_ReturnsError(t *testing.T) {
	_, err := calc.Evaluate("3 * 1/4)")
	assert.Equal(t, ErrUnbalancedParantheses, err)
}

func TestEvaluate_DivideByZero_ReturnsError(t *testing.T) {
	_, err := calc.Evaluate("1 / 0")
	assert.Equal(t, ErrDivideByZero, err)
}

func TestEvaluate_DivideByNegativeZero_ReturnsError(t *testing.T) {
	_, err := calc.Evaluate("1 / -0")
	assert.Equal(t, ErrDivideByZero, err)
}

func TestEvaluate_ComplexExpressionWithoutSpaces_EvaluatesCorrectly(t *testing.T) {
	result, _ := calc.Evaluate("3*2+4*5-6")

	assert.Equal(t, 20.0, result)
}

func TestEvaluate_ComplexExpressionWithSpaces_EvaluatesCorrectly(t *testing.T) {
	result, _ := calc.Evaluate("3 * 2 + 4 * 5 - 6")

	assert.Equal(t, 20.0, result)
}

func TestEvaluate_ComplexExpression_EvaluatesWithCorrectPrecedence(t *testing.T) {
	result, _ := calc.Evaluate("3*2+4*5-6/3")

	assert.Equal(t, 24.0, result)
}
