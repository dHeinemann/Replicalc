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

#include "../src/calc.h"
#include "../Unity/src/unity.h"

void setUp(void) {}
void tearDown(void) {}

void calculate_onePlusOne_Returns2() {
    TEST_ASSERT_EQUAL(2, calculate("1 + 1", NULL));
}

void calculate_unbalancedOpenParen_ReturnsError() {
    int errorCode = 0;
    calculate("3 * (1/4", &errorCode);
    TEST_ASSERT_EQUAL(Error_UnbalParens, errorCode);
}

void calculate_unbalancedCloseParen_ReturnsError() {
    int errorCode = 0;
    calculate("3 * 1/4)", &errorCode);
    TEST_ASSERT_EQUAL(Error_UnbalParens, errorCode);
}

void calculate_divideByZero_ReturnsError() {
    int errorCode = 0;
    calculate("1 / 0", &errorCode);
    TEST_ASSERT_EQUAL(Error_DivideByZero, errorCode);
}

void calculate_divideByNegativeZero_ReturnsError() {
    int errorCode = 0;
    calculate("1 / -0", &errorCode);
    TEST_ASSERT_EQUAL(Error_DivideByZero, errorCode);
}

void calculate_complexExpressionWithoutSpaces_EvaluatesCorrectly() {
    int errorCode;
    int result;

    errorCode = 0;
    result = calculate("3*2+4*5-6", &errorCode);

    TEST_ASSERT_EQUAL(20, result);
}

void calculate_complexExpressionWithSpaces_EvaluatesCorrectly() {
    int errorCode;
    int result;

    errorCode = 0;
    result = calculate("3 * 2 + 4 * 5 - 6", &errorCode);

    TEST_ASSERT_EQUAL(20, result);
}

void calculate_complexExpression_EvaluatesWithCorrectPrecedence() {
    int errorCode;
    int result;

    errorCode = 0;
    result = calculate("3*2+4*5-6/3", &errorCode);

    TEST_ASSERT_EQUAL(24, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(calculate_onePlusOne_Returns2);
    RUN_TEST(calculate_unbalancedOpenParen_ReturnsError);
    RUN_TEST(calculate_unbalancedCloseParen_ReturnsError);
    RUN_TEST(calculate_divideByZero_ReturnsError);
    RUN_TEST(calculate_divideByNegativeZero_ReturnsError);
    RUN_TEST(calculate_complexExpressionWithSpaces_EvaluatesCorrectly);
    RUN_TEST(calculate_complexExpressionWithoutSpaces_EvaluatesCorrectly);
    RUN_TEST(calculate_complexExpression_EvaluatesWithCorrectPrecedence);
    return UNITY_END();
}