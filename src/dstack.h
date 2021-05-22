#ifndef DSTACK_HEADER
#define DSTACK_HEADER

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
struct Double_Stack* double_stack_new(int capacity, int length);

/*
 * De-allocate memory for a double_stack.
 */
void double_stack_free(struct Double_Stack* double_stack);

/*
 * Test whether a double_stack is full.
 */
int double_stack_isFull(struct Double_Stack* double_stack);

/*
 * Test whether a double_stack is empty.
 */
int double_stack_isEmpty(struct Double_Stack* double_stack);

/*
 * Push a string onto a double_stack.
 */
void double_stack_push(struct Double_Stack* double_stack, double item);

/*
 * Remove the top-most element from the double_stack.
 */
double double_stack_pop(struct Double_Stack* double_stack);

/*
 * Get the top-most element from the double_stack.
 */
double double_stack_peek(struct Double_Stack* double_stack);

#endif
