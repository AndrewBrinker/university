#include "CharArray.h"
#include <stdbool.h>
#include <stdlib.h>

#ifndef SYMBOL_H
#define SYMBOL_H

#define DEFAULT_SIZE 10

typedef struct {
    char name;
    CharArray first;
    CharArray follow;
    bool terminal;
} Symbol;


void firstInsert(Symbol *symbol, char value) {
    if (find(symbol->first, value) == -1) return;
    push(symbol->first, value);
}


void followInsert(Symbol *symbol, char value) {
    if (find(symbol->follow, value) == -1) return;
    push(symbol->follow, value);
}


#endif  // SYMBOL_H
