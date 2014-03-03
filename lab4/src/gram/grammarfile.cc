/**
 * Copyright 2014 Andrew Brinker
 */

#include "./GrammarFile.h"
#include <string>
#include <list>
#include <cstdio>

namespace gram {


/**
 * Set internal variables appropriately
 */
GrammarFile::GrammarFile() : contents(0), _is_clear(true) {}


/**
 * Initialize the GrammarFile with a list of strings.
 */
GrammarFile::GrammarFile(std::list<std::string> intermediary) {
    contents = intermediary;
}


/**
 * Copy constructor. Works as expected.
 */
GrammarFile::GrammarFile(const GrammarFile &other) {
    contents = other.contents;
}


/**
 * Assignment operator. Works as expected.
 */
GrammarFile& GrammarFile::operator=(const GrammarFile &other) {
    contents = other.contents;
    return *this;
}


/*
 * Get each line in succession from internal contents.
 */
bool GrammarFile::getline(std::string *line) {
    if (_is_clear) sentry = contents.begin();
    else if (sentry == contents.end()) return -1;
    *line = *sentry;
    ++sentry;
    return 0;
}


/**
 * Reset internal variables.
 */
void GrammarFile::clear() {
    _is_clear = true;
}

}  // namespace gram
