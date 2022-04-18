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

func TestMain(m *testing.M) {
	InitializeVariables()
}

func TestCalculate_OnePlusOne_Returns2(t *testing.T) {
	result, _, _ := Calculate("1 + 1")
	assert.Equal(t, 2.0, result)
}

func TestCalculate_UnbalancedOpenParen_ReturnsError(t *testing.T) {
	_, errorCode, _ := Calculate("3 * (1/4")
	assert.Equal(t, ErrorUnbalancedParantheses, errorCode)
}

func TestCalculate_lnbalancedCloseParen_ReturnsError(t *testing.T) {
	_, errorCode, _ := Calculate("3 * 1/4)")
	assert.Equal(t, ErrorUnbalancedParantheses, errorCode)
}

func TestCalculate_DivideByZero_ReturnsError(t *testing.T) {
	_, errorCode, _ := Calculate("1 / 0")
	assert.Equal(t, ErrorDivideByZero, errorCode)
}

func TestCalculate_DivideByNegativeZero_ReturnsError(t *testing.T) {
	_, errorCode, _ := Calculate("1 / -0")
	assert.Equal(t, ErrorDivideByZero, errorCode)
}

func TestCalculate_ComplexExpressionWithoutSpaces_EvaluatesCorrectly(t *testing.T) {
	result, _, _ := Calculate("3*2+4*5-6")

	assert.Equal(t, 20.0, result)
}

func TestCalculate_ComplexExpressionWithSpaces_EvaluatesCorrectly(t *testing.T) {
	result, _, _ := Calculate("3 * 2 + 4 * 5 - 6")

	assert.Equal(t, 20.0, result)
}

func TestCalculate_ComplexExpression_EvaluatesWithCorrectPrecedence(t *testing.T) {
	result, _, _ := Calculate("3*2+4*5-6/3")

	assert.Equal(t, 24.0, result)
}
