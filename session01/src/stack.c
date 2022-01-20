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
    stk->top = NULL;
    stk->num_elements = 0;
}

/**
 * Pushes a new element to the stack. We allocate a stack_node,
 * assign its fields, then add it to the stack.
 */
void stack_push(stack *stk, struct student *stu) {
    struct stack_node *sn = malloc(sizeof(struct stack_node));
    if (sn == NULL) {
        perror("Out of memory!");
        exit(1);
    }

    // First check to see if the stack is empty. If so,
    // don't change any ptrs except the top.
    if (!stack_is_empty(stk)) {        
        sn->next = stk->top;
    }

    sn->s = stu;
    stk->top = sn;
    stk->num_elements++;
}

/**
 * Removes (but does not return) the top of the stack.
 */
void stack_pop(stack *stk) {
    struct stack_node *curr = stk->top;
    stk->top = stk->top->next;
    free(curr->s);
    free(curr);
    stk->num_elements--;
}

/**
 * Returns (but does not remove) the top element of the stack.
 */
struct student *stack_peek(stack *stk) {
    if (stack_is_empty(stk)) {
        perror("Stack is empty!");
        exit(1);
    }
    
    return stk->top->s;
}

/**
 * Returns whether or not the stack is empty.
 */
bool stack_is_empty(stack *stk) {
    return stack_num_elements(stk) == 0;
}

/**
 * Returns the number of elements in the stack.
 */
int stack_num_elements(stack *stk) {
    return stk->num_elements;
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

    while (curr != NULL) {
        next = curr->next;
        free(curr->s);
        free(curr);
        curr = next;
    }
}
