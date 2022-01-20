#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "student.h"

struct stack_node {
    struct student *s;
    struct stack_node *next;
};

/**
 * Initializes a stack to have no elements (and no top ptr).
 */
void stack_init(stack *stk) {
    // TODO write the code to initialize the stack.
}

/**
 * Pushes a new element to the stack. We allocate a stack_node,
 * assign its fields, then add it to the stack.
 */
void stack_push(stack *stk, struct student *stu) {
    // TODO write the code to push an element to the stack.
}

/**
 * Removes (but does not return) the top of the stack.
 */
void stack_pop(stack *stk) {
    // TODO write the code to remove the top element from the stack.

/**
 * Returns (but does not remove) the top element of the stack.
 */
struct student *stack_peek(stack *stk) {
    // TODO write the code to return the top element on the stack.
}

/**
 * Returns whether or not the stack is empty.
 */
bool stack_is_empty(stack *stk) {
    // TODO write the code to determine if the stack is empty.
}

/**
 * Returns the number of elements in the stack.
 */
int stack_num_elements(stack *stk) {
    // TODO write the code to return the number of elements.
}

/**
 * Prints the stack in top-to-bottom style (i.e., the top is printed first
 * which works its way to the bottom of the stack).
 */
void stack_print(stack *stk, FILE *ostream) {
    if (stack_is_empty(stk)) {
        perror("Stack is empty!");
        exit(1);
    }

    struct stack_node *curr = stk->top;
    while (curr != NULL) {
        student_print(curr->s, ostream);
        curr = curr->next;
    }
}

/**
 * Frees all data/student elements in the stack.
 */
void stack_free(stack *stk) {
    struct stack_node *curr = stk->top;
    struct stack_node *next = curr;

    // TODO write the loop to free the nodes AND the student structs.
}