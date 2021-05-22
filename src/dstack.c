#include <stdlib.h>

/*
 * Represents a double_stack of strings.
 */
struct Double_Stack {
    int top;
    int capacity;
    double* array;
};

/*
 * Create a new double_stack.
 */
struct Double_Stack* double_stack_new(int capacity, int length) {
    struct Double_Stack* double_stack = (struct Double_Stack*) malloc(sizeof(struct Double_Stack));
    double_stack->capacity = capacity;
    double_stack->top = -1;
    double_stack->array = (double*) malloc(sizeof(double[capacity][length]));

    return double_stack;
}

/*
 * De-allocate memory for a double_stack.
 */
void double_stack_free(struct Double_Stack* double_stack) {
    free(double_stack->array);
    free(double_stack);
}

/*
 * Test whether a double_stack is full.
 */
int double_stack_isFull(struct Double_Stack* double_stack) {
    return double_stack->top == double_stack->capacity -1;
}

/*
 * Test whether a double_stack is empty.
 */
int double_stack_isEmpty(struct Double_Stack* double_stack) {
    return double_stack->top == -1;
}

/*
 * Push a string onto a double_stack.
 */
void double_stack_push(struct Double_Stack* double_stack, double item) {
    if (double_stack_isFull(double_stack)) {
        return;
    }

    double_stack->array[++double_stack->top] = item;
}

/*
 * Remove the top-most element from the double_stack.
 */
double double_stack_pop(struct Double_Stack* double_stack) {
    if (double_stack_isEmpty(double_stack)) {
        return -1; /* todo */
    }

    return double_stack->array[double_stack->top--];
}

/*
 * Get the top-most element from the double_stack.
 */
double double_stack_peek(struct Double_Stack* double_stack) {
    if (double_stack_isEmpty(double_stack)) {
        return -1; /* todo */
    }

    return double_stack->array[double_stack->top];
}
