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
#include <string>

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
        printf("    ID: %d\n", lrset.identifier);
        printf("    Transition: %c\n", lrset.transition_code);
        for (auto item : data) {
            printf("        Production: [%d] %s\n", item.dot,
                                                    item.production.c_str());
        }
        printf("\n");
    }
}


void print_productions(const std::set<std::string> input) {
    for (auto production : input) {
        printf("    %s\n", production.c_str());
    }
}


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }

    Parser p(argv[1]);
    p.parse();

    printf("\nProductions:\n");
    print_productions(p.productions());

    printf("\nCanonical LR(0) Sets:\n\n");
    print_lrsets(p.canon());

    return 0;
}
