#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#include "stack.h"
#include "symbol.h"

static stack stk;

/**
 * Evaluates a unary operator. We expect the top element of the stack to be 
 * the operator with the next being an operand.
 */
char evaluate_unaryop(void) {
    // Check the stack size first.
    int num_elements;
    if ((num_elements = stack_num_elements(&stk)) < 2) {
        fprintf(stderr, "Expected two symbols in stack, but there are only %d\n", num_elements);
        exit(EXIT_FAILURE);
    }

    // Remove elements and extract data.
    struct symbol *op = stack_remove(&stk);
    struct symbol *operand1 = stack_remove(&stk);
    char operator = op->data.operator.ch;
    bool op1 = operand1->data.value;
        
    // Before we evaluate, we need to free the data.
    symbol_free(operand1);
    symbol_free(op);
    
    switch (operator) {
        case '~': return symbol_convert(!op1);
        default:
            fprintf(stderr, "Invalid unary operator %c\n", operator);
            exit(EXIT_FAILURE);
    }
}

/**
 * Evaluates a binary operator. We extract the first three elements of the stack,
 * expecting the top element to be the operator and the next two ought to be 
 * operands (t/f).
 */
char evaluate_binop(void) {
    // Check the stack size first.
    int num_elements;
    if ((num_elements = stack_num_elements(&stk)) < 3) {
        fprintf(stderr, "Expected three symbols in stack, but there are only %d\n", num_elements);
        exit(EXIT_FAILURE);
    }

    // We expect the top of the stack to be the operator and the next two to be operands.
    struct symbol *op = stack_remove(&stk);
    struct symbol *operand1 = stack_remove(&stk);
    struct symbol *operand2 = stack_remove(&stk);
    char operator = op->data.operator.ch;
    bool op1 = operand2->data.value;
    bool op2 = operand1->data.value;
        
    // Before we evaluate, we need to free the data.
    symbol_free(operand2);
    symbol_free(operand1);
    symbol_free(op);
    
    switch (operator) {
        case '>': return symbol_convert(!op1 || op2);
        case '+': return symbol_convert(op1 || op2);
        case '&': return symbol_convert(op1 && op2);
        case '^': return symbol_convert(op1 != op2);
        case '#': return symbol_convert((op1 && op2) || (!op1 && !op2));
        default:
            fprintf(stderr, "Invalid binary operator %c\n", operator);
            exit(EXIT_FAILURE);
    }
}

/**
 * Parses a line of data. This should be a boolean expression in post-fix notation.
 */
void parse(char *line_data) {
    char *tok = strtok(line_data, " ");
    while (tok != NULL) {
        // Push the current symbol to the stack.
        stack_push(&stk, symbol_init(tok[0]));

        // Attempt to evaluate whatever is at the top.
        struct symbol *top = stack_peek(&stk);
        if (!top->is_literal) {
            switch (top->data.operator.op_type) {
                case BINARY:
                    stack_push(&stk, symbol_init(evaluate_binop()));
                    break;
                case UNARY:
                    stack_push(&stk, symbol_init(evaluate_unaryop()));
                    break;
            }
        }

        tok = strtok(NULL, " ");
    }

    if (!stack_is_empty(&stk)) {
        struct symbol *s = stack_remove(&stk);
        symbol_print(s, stdout);
        symbol_free(s);
    } 
}

/**
 * Problem Statement: Write a program that evaluates simple propositional logic expressions.
 * The user will enter expressions in postfix, meaning t f > evaluates to T -> F (true implies
 * false). Use a stack to push operands and operators.
 * 
 * Example: t f > f # t ^ ~ evaluates to true because
 *  1) t f > is false
 *  2) f f # is true
 *  3) t t ^ is false
 *  4) f ~ is true
 * 
 * Let X and Y be two propositional logic literals (true or false).
 * Possible Operands: 
 *  + - Disjunction (true iff at least one of X and Y are true).
 *  & - Conjunction (true iff X and Y are both true).
 *  > - Implication (true iff X is false or Y is true).
 *  ^ - Exclusive-Or (true iff X and Y are different).
 *  # - Biconditional (true iff X and Y are the same).
 *  ~ - Negation (true iff X is false)
 */
int main(int argc, char *argv[]) {
    // Initialize the stack.
    stack_init(&stk);

    // Main input loop.
    char line[LINE_MAX];
    while (fgets(line, LINE_MAX, stdin) != NULL) {
        // Trim the newline.
        line[strlen(line) - 1] = '\0';

        // Parse the boolean expression.
        parse(line);
    }

    stack_free(&stk);
}