/*
The MIT License (MIT)

Copyright (c) 2013 Andrew Brinker

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
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
