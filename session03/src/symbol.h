#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>
#include <stdio.h>

enum OP_TYPE {
    UNARY,
    BINARY
};

struct symbol {
    // TODO create the symbol struct. It should have a union of "data": 
    //      either a bool value or a struct called operator. The struct
    //      will contain either a char representing the operator itself
    //      and an enum representing the type.
    //
    //      There should also be a bool keeping track of whether or not 
    //      this symbol is a literal.
};

struct symbol *symbol_init(const char s);
char symbol_convert(const bool val);
void symbol_print(struct symbol *s, FILE *ostream);
void symbol_free(struct symbol *s);

#endif // SYMBOL_H