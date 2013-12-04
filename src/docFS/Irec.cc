/*
 * Copyright 2013 Andrew Brinker
 */

#include <string>
#include "./Irec.h"

/*
 * Irec()
 *
 * @in: std::string new_key
 *   - The key of the Irec
 * @in: unsigned int new_block_id
 *   - The block ID of the Irec
 *
 * Make a new Irec with the specified values.
 */
Irec::Irec(std::string new_key, unsigned int new_block_id)
           :key(new_key), block_id(new_block_id) {}
