/*
 * Copyright 2013 Andrew Brinker
 */

#include <sstream>
#include <string>
#include <vector>
#include "./Bnode.h"

#define FILL_CHAR '#'

Bnode::Bnode(std::string new_buffer) {
  buffer = new_buffer;
  std::istringstream instream;
  instream.str(buffer);
  unsigned int vsize = 0;
  instream >> vsize;
  std::string k;
  unsigned int b = 0;
  for (unsigned int i = 0; i < vsize; ++i) {
    instream >> k >> b;
    keys.push_back(k);
    block_ids.push_back(b);
  }
}


Bnode::Bnode(std::vector<std::string> new_keys,
             std::vector<unsigned int> new_block_ids,
             unsigned int new_block_size) {
  keys = new_keys;
  block_ids = new_block_ids;
  std::ostringstream outstream;
  outstream << keys.size() << " ";
  for (unsigned int i = 0; i < keys.size(); ++i) {
    outstream << keys[i] << " " << block_ids[i] << " ";
  }
  buffer = outstream.str();
  for (unsigned int i = buffer.length(); i < new_block_size; ++i) {
    buffer.push_back(FILL_CHAR);
  }
}
