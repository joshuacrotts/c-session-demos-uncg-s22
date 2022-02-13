#ifndef LVAL_H
#define LVAL_H

#include "mpc.h"
#include "env.h"

#include <stdbool.h>

enum LVAL_TYPE {
    LVAL_NUMBER,
    LVAL_STRING,
    LVAL_CHAR,
    LVAL_SYMBOL,
    LVAL_BOOL,
    LVAL_TREE,
    LVAL_PROCEDURE
};

typedef struct lval {
    union data {
        long double num_value;
        char ch_value;
        bool bool_value;
        char *str_value;
        char *sym_value;
        void *tree_value;
        struct proc_value {
            struct environment *proc_env;
            struct lval *proc_name;
            struct lval **proc_parameters;
            mpc_ast_t *proc_body;
            size_t proc_num_parameters;
        } proc_val;     
    } data;

    enum LVAL_TYPE type;
} lval;

lval *lval_init_number(long double val);
lval *lval_init_symbol(char *sym);
lval *lval_init_procedure(lval *id, lval **parameters, mpc_ast_t *body, size_t num_parameters, struct environment *env);
void lval_print(const lval *lv);
void lval_delete(lval *lv);

#endif // LVAL_H