/**
 * Copyright 2014 Andrew Brinker
 */

#include "./Grammar.h"
#include <string>
#include <list>
#include <cstdio>


/**
 * Set internal variables appropriately
 */
Grammar::Grammar() : contents(0) {}


/**
 * Initialize the Grammar with a list of strings.
 */
Grammar::Grammar(std::list<std::string> intermediary) {
    contents = intermediary;
}


/**
 * Copy constructor. Works as expected.
 */
Grammar::Grammar(const Grammar &other) {
    contents = other.contents;
}


/**
 * Assignment operator. Works as expected.
 */
Grammar& Grammar::operator=(const Grammar &other) {
    contents = other.contents;
    return *this;
}
