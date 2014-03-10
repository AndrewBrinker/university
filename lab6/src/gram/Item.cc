/**
 * Copyright 2014 Andrew Brinker
 */

#include "./Item.h"
#include <string>

/**
 * Initialize an Item
 */
Item::Item(std::string p, int d) {
    production = p;
    dot = d;
}


/**
 * Copy the contents of other to this.
 * @param  other -> The thing to be copied
 * @return a reference to the current object
 */
Item::Item(const Item &other) {
    production = other.production;
    dot = other.dot;
}


/**
 * Assign the contents of other to this.
 * @param  other -> The thing being copied
 * @return a reference to the current object
 */
Item& Item::operator=(const Item &other) {
    production = other.production;
    dot = other.dot;
    return *this;
}

/**
 * Compare two Items
 * @return which one is smaller
 */
bool Item::operator<(const Item& other) const {
    return production < other.production;
}
