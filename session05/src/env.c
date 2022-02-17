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
    symbol_entry *symbol = s_malloc(sizeof(symbol_entry));
    symbol->next = NULL;
    symbol->value = value;
    symbol->key = s_malloc(strlen(key) + 1);
    strcpy(symbol->key, key);
    symbol->key[strlen(key)] = '\0'; 
    if (env->head == NULL) {
        env->head = env->tail = symbol;
    } else {
        env->tail->next = symbol;
        env->tail = symbol;
    }
}

/**
 * Looks up the symbol @key in the @env. If it is not present, we 
 * search its parent environment. If it's not there, we just return NULL.
 */
lval *environment_lookup(const environment *env, const char *key) {
    lval *lv = NULL;
    for (symbol_entry *curr = env->head; curr != NULL; curr = curr->next) {
        if (streq(key, curr->key)) {
            return curr->value;
        }
    }

    if (env->parent != NULL) {
        return environment_lookup(env->parent, key);
    }

    return NULL;
}

/**
 * Appends the formals @formals to their arguments @args. This just calls environment_init 
 * on a new environment with @parent as its parent, and we bind the args to the parameters.
 */
environment *environment_append(environment *parent, lval **formals, lval **args, size_t num_operands) {
    environment *curr_env = environment_init(parent);
    for (int i = 0; i < num_operands; i++) {
        environment_bind(curr_env, formals[i]->data.sym_value, args[i]);
    }
    return curr_env;
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
