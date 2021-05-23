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

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(calculate_onePlusOne_Returns2);
    RUN_TEST(calculate_unbalancedOpenParen_ReturnsError);
    RUN_TEST(calculate_unbalancedCloseParen_ReturnsError);
    RUN_TEST(calculate_divideByZero_ReturnsError);
    return UNITY_END();
}