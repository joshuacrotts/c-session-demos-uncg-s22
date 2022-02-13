#ifndef APPLY_H
#define APPLY_H

#include <stddef.h>

#include "lval.h"

lval *apply(lval *op, lval **operands, size_t num_args);

#endif // APPLY_H