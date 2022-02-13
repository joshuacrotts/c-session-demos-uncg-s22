#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "apply.h"
#include "utils.h"

static lval *apply_add(lval **operands, size_t num_operands);
static lval *apply_subtract(lval **operands, size_t num_operands);
static lval *apply_multiply(lval **operands, size_t num_operands);
static lval *apply_divide(lval **operands, size_t num_operands);

lval *apply(lval *op, lval **operands, size_t num_operands) {
    switch (op->type) {
        case LVAL_SYMBOL: {
            char *sym = op->data.sym_value;
            if (streq(sym, "+")) {
                return apply_add(operands, num_operands);
            } else if (streq(sym, "-")) {
                return apply_subtract(operands, num_operands);
            } else if (streq(sym, "*")) {
                return apply_multiply(operands, num_operands);
            } else if (streq(sym, "/")) {
                return apply_divide(operands, num_operands);
            }
        }
        default:
            fprintf(stderr, "Cannot apply on operator of type %d\n", op->type);
            exit(1);
    }
    return NULL;
}

static lval *apply_add(lval **operands, size_t num_operands) {
    if (num_operands == 0) {
        fprintf(stderr, "+ expected at least one argument but got 0\n");
    }
    long double val = 0;
    for (int i = 0; i < num_operands; i++) {
        val += operands[i]->data.num_value;
    }
    return lval_init_number(val);
}

static lval *apply_subtract(lval **operands, size_t num_operands) {
    if (num_operands == 0) {
        fprintf(stderr, "- expected at least one argument but got 0\n");
    }
    if (num_operands == 1) {
        return lval_init_number(-operands[0]->data.num_value);
    }
    long double val = operands[0]->data.num_value;
    for (int i = 1; i < num_operands; i++) {
        val -= operands[i]->data.num_value;
    }
    return lval_init_number(val);
}

static lval *apply_multiply(lval **operands, size_t num_operands) {
    long double val = 1;
    for (int i = 0; i < num_operands; i++) {
        val *= operands[i]->data.num_value;
    }
    return lval_init_number(val);
}

static lval *apply_divide(lval **operands, size_t num_operands) {
    if (num_operands != 2) {
        fprintf(stderr, "ERR / expects 2 arguments but got %zu\n", num_operands);
    }
    lval *dividend = operands[0];
    lval *divisor = operands[1];
    return lval_init_number(dividend->data.num_value / divisor->data.num_value);
}
