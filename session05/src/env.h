#ifndef ENV_H
#define ENV_H

#include "lval.h"

typedef struct symbol_entry {
    char *key;
    struct lval *value;
    struct symbol_entry *next;
} symbol_entry;

typedef struct environment {
    struct environment *parent;
    struct symbol_entry *head;
    struct symbol_entry *tail;
} environment;

environment *environment_init(environment *parent);
void environment_bind(environment *env, const char *key, struct lval *value);
struct lval *environment_lookup(const environment *env, const char *key);
environment *environment_append(environment *parent, struct lval **formals, struct lval **args, size_t num_operands);
void environment_delete(environment *env);

#endif // ENV_H