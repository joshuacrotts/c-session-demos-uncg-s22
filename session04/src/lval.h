#ifndef LVAL_H
#define LVAL_H

#include <stdbool.h>

enum LVAL_TYPE {
    LVAL_NUMBER,
    LVAL_STRING,
    LVAL_CHAR,
    LVAL_SYMBOL,
    LVAL_BOOL,
    LVAL_TREE
};

typedef struct lval {
    union data {
        long double num_value;
        char ch_value;
        bool bool_value;
        char *str_value;
        char *sym_value;
        void *tree_value;        
    } data;

    enum LVAL_TYPE type;
} lval;

lval *lval_init_number(long double val);
lval *lval_init_symbol(char *sym);
void lval_print(const lval *lv);
void lval_delete(lval *lv);

#endif // LVAL_H