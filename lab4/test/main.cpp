#include <Grammar/grammar.h>
#include <set>
#include <iostream>
#include <vector>
#include <string>

template<typename T>
void print_set(std::set<T> input) {
    for (auto it = input.begin(); it != input.end(); ++it) {
        std::cout << *it << std::endl;
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

    return 0;
}