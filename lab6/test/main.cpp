/**
 * Copyright 2014 Andrew Brinker
 */

#include <gram/Parser.h>
#include <gram/LRSet.h>
#include <gram/Item.h>
#include <set>
#include <cstdio>
#include <cctype>
#include <map>

void print_mapset(const std::map<char, std::set<char>> input) {
    for (auto pair : input) {
        if (isupper(pair.first)) {
            printf("    %c => ", pair.first);
            for (auto item : pair.second) printf("%c ", item);
            printf("\n");
        }
    }
}


void print_lrsets(const std::set<LRSet> input) {
    for (auto lrset : input) {
        auto data = lrset.data;
        printf("    Identifier:        %d\n", lrset.identifier);
        printf("    Transition Code:   %c\n", lrset.transition_code);
        printf("    Productions:\n");
        for (auto item : data) {
            printf("        Production:      %s\n", item.production.c_str());
            printf("        Dot Position:    %d\n", item.dot);
        }
    }
}


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }

    Parser p(argv[1]);
    p.parse();

    printf("\nFIRST:\n");
    print_mapset(p.first());

    printf("\nFOLLOW:\n");
    print_mapset(p.follow());

    printf("\nCanonical LR(0) Sets:\n");
    print_lrsets(p.canon());

    return 0;
}
