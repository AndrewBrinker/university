/**
 * Copyright 2014 Andrew Brinker
 */

#include <gram/grammar.h>
#include <set>
#include <cstdio>
#include <cctype>
#include <map>

using namespace gram;

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

    grammar g(argv[1]);
    g.parse();
    printf("FIRST:\n");
    print_mapset(g.first());
    printf("FOLLOW:\n");
    print_mapset(g.follow());

    return 0;
}
