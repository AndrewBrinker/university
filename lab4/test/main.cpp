/**
 * Copyright 2014 Andrew Brinker
 */

#include <Grammar/Grammar.h>
#include <set>
#include <cstdio>
#include <cctype>
#include <map>


void print_mapset(std::map<char, std::set<char>> input) {
    for (auto pair : input) {
        if (isupper(pair.first)) {
            printf("    %c => ", pair.first);
            for (auto item : pair.second) printf("%c ", item);
            printf("\n");
        }
    }
}


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }

    Grammar grammar(argv[1]);
    grammar.parse();
    printf("FIRST:\n");
    print_mapset(grammar.first());
    printf("FOLLOW:\n");
    print_mapset(grammar.follow());

    return 0;
}
