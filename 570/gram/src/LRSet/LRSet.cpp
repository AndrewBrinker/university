/**
 * Copyright 2014 Andrew Brinker
 */

#include "./LRSet.h"
#include <Item/Item.h>
#include <set>

/**
 * Initialize an LRSet
 */
LRSet::LRSet(std::set<Item> d, int i, char t) {
    data = d;
    identifier = i;
    transition_code = t;
}


/**
 * Copy the contents of other to this.
 * @param  other -> The thing to be copied
 * @return a reference to the current object
 */
LRSet::LRSet(const LRSet &other) {
    data = other.data;
    identifier = other.identifier;
    transition_code = other.transition_code;
}


/**
 * Assign the contents of other to this.
 * @param  other -> The thing being copied
 * @return a reference to the current object
 */
LRSet& LRSet::operator=(const LRSet &other) {
    data = other.data;
    identifier = other.identifier;
    transition_code = other.transition_code;
    return *this;
}


/**
 * Compare two LRSets
 * @return which one is smaller
 */
bool LRSet::operator<(const LRSet &other) const {
    return identifier < other.identifier;
}
