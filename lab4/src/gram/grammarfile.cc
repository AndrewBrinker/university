/**
 * Copyright 2014 Andrew Brinker
 */

#include "./grammarfile.h"
#include <string>
#include <list>
#include <cstdio>

namespace gram {


/**
 * Set internal variables appropriately
 */
grammarfile::grammarfile() : contents(0), _is_clear(true) {}


/**
 * Initialize the grammarfile with a list of strings.
 */
grammarfile::grammarfile(std::list<std::string> intermediary) {
    contents = intermediary;
}


/**
 * Copy constructor. Works as expected.
 */
grammarfile::grammarfile(const grammarfile &other) {
    contents = other.contents;
}


/**
 * Assignment operator. Works as expected.
 */
grammarfile& grammarfile::operator=(const grammarfile &other) {
    contents = other.contents;
    return *this;
}


/*
 * Get each line in succession from internal contents.
 */
bool grammarfile::getline(std::string *line) {
    if (_is_clear) sentry = contents.begin();
    else if (sentry == contents.end()) return -1;
    *line = *sentry;
    ++sentry;
    return 0;
}


/**
 * Reset internal variables.
 */
void grammarfile::clear() {
    _is_clear = true;
}

}  // namespace gram
