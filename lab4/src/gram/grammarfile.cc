/**
 * Copyright 2014 Andrew Brinker
 */

#include "./grammarfile.h"
#include <string>
#include <list>
#include <cstdio>

namespace gram {

grammarfile::grammarfile() : contents(0), _is_clear(true) {}

grammarfile::grammarfile(std::list<std::string> intermediary) {
    contents = intermediary;
}

grammarfile::grammarfile(const grammarfile &other) {
    contents = other.contents;
}

grammarfile& grammarfile::operator=(const grammarfile &other) {
    contents = other.contents;
    return *this;
}

bool grammarfile::getline(std::string *line) {
    if (_is_clear) sentry = contents.begin();
    else if (sentry == contents.end()) return -1;
    *line = *sentry;
    ++sentry;
    return 0;
}

void grammarfile::clear() {
    _is_clear = true;
}

}  // namespace gram
