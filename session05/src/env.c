#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env.h"
#include "utils.h"

environment *environment_init(environment *parent) {
    environment *env = s_malloc(sizeof(environment));
    env->parent = parent;
    env->head = env->tail = NULL;
    return env;
}

/**
 * Creates a new binding in the environment. We need to dynamically-allocate the
 * symbol's key and then strcpy it over. The linked list is updated accordingly.
 */
void environment_bind(environment *env, const char *key, lval *value) {
    // TODO write the "bind" procedure.
}

/**
 * Looks up the symbol @key in the @env. If it is not present, we 
 * search its parent environment. If it's not there, we just return NULL.
 */
lval *environment_lookup(const environment *env, const char *key) {
    // TODO write the "lookup" procedure.
    return NULL;
}

/**
 * Perpends the formals @formals to their arguments @args. This just calls environment_init 
 * on a new environment with @parent as its parent, and we bind the args to the parameters.
 */
environment *environment_perpend(environment *parent, lval **formals, lval **args, size_t num_operands) {
    // TODO write the "perpend" procedure.
}

void environment_delete(environment *env) {
    symbol_entry *curr = env->head;
    while (curr != NULL) {
        symbol_entry *tmp = curr;
        curr = curr->next;
        lval_delete(tmp->value);
        free(tmp);
    }
}
