#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>
#include <stdio.h>

enum OP_TYPE {
    UNARY,
    BINARY
};

struct symbol {
    union data {
        bool value;
        struct operator {
            char ch;
            enum OP_TYPE op_type;
        } operator;
    } data;
    bool is_literal;
};

struct symbol *symbol_init(const char s);
char symbol_convert(const bool val);
void symbol_print(struct symbol *s, FILE *ostream);
void symbol_free(struct symbol *s);

#endif // SYMBOL_H