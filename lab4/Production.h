#include "Symbol.h"

#ifndef PRODUCTION_H
#define PRODUCTION_H

typedef struct {
    Symbol lhs;
    Symbol *rhs;
} Production;

#endif  // PRODUCTION_H
