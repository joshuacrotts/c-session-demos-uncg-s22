#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "lval.h"
#include "utils.h"

lval *lval_init_number(long double data) {
    lval *lv = s_malloc(sizeof(lval));
    lv->type = LVAL_NUMBER;
    lv->data.num_value = data;
    return lv;
}

lval *lval_init_symbol(char *data) {
    lval *lv = s_malloc(sizeof(lval));
    lv->type = LVAL_SYMBOL;
    lv->data.sym_value = s_malloc(strlen(data) + 1);
    strcpy(lv->data.sym_value, data);
    lv->data.sym_value[strlen(data)] = '\0';
    return lv;
}

lval *lval_init_procedure(lval *id, lval **parameters, mpc_ast_t *body, size_t num_parameters, struct environment *env) {
    lval *lv = s_malloc(sizeof(lval));
    lv->type = LVAL_PROCEDURE;
    lv->data.proc_val.proc_name = id;
    lv->data.proc_val.proc_body = body;
    lv->data.proc_val.proc_parameters = parameters;
    lv->data.proc_val.proc_num_parameters = num_parameters;
    lv->data.proc_val.proc_env = env;
    return lv;
}

void lval_print(const lval *lv) {
    if (lv == NULL) {
        fprintf(stderr, "cannot print null lval\n");
        exit(1);
    }
    switch (lv->type) {
        case LVAL_NUMBER:
            // If it's able to be represented as an integer, do that.
            if (lv->data.num_value == (int) (lv->data.num_value)) {
                printf("%d\n", (int) lv->data.num_value);
            } else {
                printf("%Le\n", lv->data.num_value);
            }
            break;
        case LVAL_SYMBOL: {
                printf("%s\n", lv->data.sym_value);
            }
            break;
        case LVAL_PROCEDURE: {
                printf("%s\n", lv->data.proc_val.proc_name->data.sym_value);
            }
            break;
        default:
            fprintf(stderr, "Cannot print node type %d\n", lv->type);
            exit(1);
    }
}

void lval_delete(lval *lv) {
    // Free any elements used...
    switch (lv->type) {
        case LVAL_SYMBOL:
            free(lv->data.sym_value);
            break;
        case LVAL_PROCEDURE:
            lval_delete(lv->data.proc_val.proc_name);
            size_t num_procedures = lv->data.proc_val.proc_num_parameters;
            for (int i = 0; i < num_procedures; i++) {
                lval_delete(lv->data.proc_val.proc_parameters[i]);
            }
            free(lv->data.proc_val.proc_parameters);
        default:
            break;
    }
    free(lv);
}
