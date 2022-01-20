#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "student.h"

typedef struct stack {
    struct stack_node *top;
    int num_elements;
} stack;

void stack_init(stack *stk);
void stack_push(stack *stk, struct student *s);
void stack_pop(stack *stk);
int stack_num_elements(stack *stk);
bool stack_is_empty(stack *stk);
struct student *stack_peek(stack *stk);
void stack_print(stack *stk, FILE *ostream);
void stack_free(stack *stk);

#endif // STACK_H