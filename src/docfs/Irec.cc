/*
 * Copyright 2013 Andrew Brinker
 */

#include <string>
#include "./Irec.h"

Irec::Irec(std::string new_key, unsigned int new_block_id)
           :key(new_key), block_id(new_block_id) {}
