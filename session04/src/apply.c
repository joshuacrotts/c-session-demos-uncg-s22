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
    // TODO finish the add function
}

static lval *apply_subtract(lval **operands, size_t num_operands) {
    // TODO finish the subtract function
}

static lval *apply_multiply(lval **operands, size_t num_operands) {
    // TODO finish the multiply function
}

static lval *apply_divide(lval **operands, size_t num_operands) {
    // TODO finish the divide function - should only have two operands
}
