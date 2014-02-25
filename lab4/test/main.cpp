/**
 * Copyright 2014 Andrew Brinker
 */

#include <Grammar/Grammar.h>
#include <set>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

void print_mapset(mapset input) {
    for (auto pair : input) {
        printf("%c => { ", pair.first);
        for (auto item : pair.second) printf("%c ", item);
        printf("}\n");
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }

    Grammar grammar(argv[1]);
    grammar.parse();
    mapset results = grammar.first();
    print_mapset(results);

    return 0;
}
