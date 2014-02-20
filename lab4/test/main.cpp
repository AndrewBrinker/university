#include <Grammar/grammar.h>
#include <set>
#include <iostream>
#include <vector>
#include <string>

void print_mapset(mapset input) {
    for (auto it = input.begin(); it != input.end(); ++it) {
        std::cout << it->first << " => {";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
            std::cout << *it2;
        }
        std::cout << "}" << std::endl;
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 0;
    }

    Grammar grammar;
    grammar.load(argv[1]);
    grammar.parse();
    mapset results = grammar.first();
    print_mapset(results);

    return 0;
}