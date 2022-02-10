#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void lval_print(const lval *lv) {
    switch (lv->type) {
        case LVAL_NUMBER:
            // If it's able to be represented as an integer, do that.
            if (lv->data.num_value == (int) (lv->data.num_value)) {
                printf("%d\n", (int) lv->data.num_value);
            } else {
                printf("%Le\n", lv->data.num_value);
            }
            break;
        case LVAL_SYMBOL:
            printf("%s\n", lv->data.sym_value);
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
        default:
            break;
    }
    free(lv);
}
