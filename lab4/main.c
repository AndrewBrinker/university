#include "Symbol.h"
#include "Production.h"
#include "Grammar.h"
#include <stdio.h>

#define EPSILON 'e'

int main(int argc, char **argv) {
    Symbol epsilon;
    epsilon.name = EPSILON;
    printf("Epsilon: '%c'\n", epsilon.name);
    return 0;
}
