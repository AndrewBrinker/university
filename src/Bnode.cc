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
    key.push_back(k);
    block_id.push_back(b);
  }
}

Bnode::Bnode(std::vector<std::string> new_key,
             std::vector<unsigned int> new_block_id,
             unsigned int new_block_size) {
  key = new_key;
  block_id = new_block_id;
  std::ostringstream outstream;
  outstream << key.size() << " ";
  for (unsigned int i = 0; i < key.size(); ++i) {
    outstream << key[i] << " " << block_id[i] << " ";
  }
  buffer = outstream.str();
  for (unsigned int i = buffer.length(); i < new_block_size; ++i) {
    buffer.push_back(FILL_CHAR);
  }
}
