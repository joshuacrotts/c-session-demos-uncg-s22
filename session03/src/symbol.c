#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "symbol.h"

static enum OP_TYPE symbol_get_optype(const char val);

/**
 * Initializes and returns a symbol with the value val. If it is 
 * t, T, f, or F, it is a boolean literal. Otherwise, it is an operator.
 */
struct symbol* symbol_init(const char val) {
    struct symbol *s = NULL;
    if ((s = malloc(sizeof(struct symbol))) == NULL) {
        fprintf(stderr, "Failed to malloc symbol ptr\n");
        exit(EXIT_FAILURE);
    }

    if (val == 't' || val == 'T' || val == 'f' || val == 'F') {
        s->is_literal = true;
        s->data.value = val == 't' || val == 'T';
    } else {
        s->is_literal = false;
        s->data.operator.op_type = symbol_get_optype(val);
        s->data.operator.ch = val;
    }

    return s;
}

/**
 * Converts a boolean symbol from the boolean literal to either t or f.
 */
char symbol_convert(const bool val) {
    return val ? 't' : 'f';
}

/**
 * Prints a symbol out to the ostream with a newline.
 */
void symbol_print(struct symbol *s, FILE *ostream) {
    if (s->is_literal) {
        fprintf(ostream, "%s\n", s->data.value ? "true" : "false");
    } else {
        fprintf(ostream, "%c\n", s->data.operator.ch);
    }
}

/**
 * Frees the provided symbol.
 */
void symbol_free(struct symbol *s) {
    free(s);
}

/**
 * Returns the "type" of the operator (i.e., whether it is unary or binary).
 */
static enum OP_TYPE symbol_get_optype(const char op) {
    switch (op) {
        case '>':
        case '^':
        case '+':
        case '&':
        case '#':
            return BINARY;
        case '~':
            return UNARY;
        default:
            fprintf(stderr, "Unknown operator %c\n", op);
            exit(EXIT_FAILURE);
    }
}