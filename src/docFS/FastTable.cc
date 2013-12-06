/*
 * Copyright 2013 Andrew Brinker
 */

/*
 * NOTE:
 *
 * This code is non-functional, and is left here only as a reminder of what
 * could have been.
 */

#include <string>
#include <vector>
#include <sstream>
#include "./FastTable.h"


Bnode::Bnode(std::string new_buffer) {
  buffer = new_buffer;
  std::istringstream instream(buffer);
  int vsize = 0;
  instream >> vsize;
  std::string k;
  int b = 0;
  for (int i = 0; i < vsize; ++i) {
    instream >> k >> b;
    keys.push_back(k);
    block_ids.push_back(b);
  }
}


Bnode::Bnode(std::vector<std::string> new_keys,
             std::vector<int> new_block_ids,
             int new_block_size) {
  keys = new_keys;
  block_ids = new_block_ids;
  std::ostringstream outstream;
  outstream << keys.size() << " ";
  for (unsigned int i = 0; i < keys.size(); ++i) {
    outstream << keys[i] << " " << block_ids[i] << " ";
  }
  buffer = outstream.str();
  for (int i = buffer.length(); i < new_block_size; ++i) {
    buffer.push_back(FILL_CHAR);
  }
}


FastTable::FastTable(std::string new_diskname,
                     std::string new_flat_file,
                     std::string new_index_file)
                   : Table(new_diskname,
                           new_flat_file,
                           new_index_file) {
  makeFile(new_flat_file);
  makeFile(new_index_file);
}


int FastTable::buildTable(std::string inputFile) {
  std::string temp = inputFile;
  return 1;
}

/*
void FastTable::BuildBtree(){
  // Get the index records off the sequental index file
  // Let key1 blockid1, and key2 blockid2 be the first two index records
  Buildroot(Irec(key1,blockid1).Irec(key2, blockid2));

  for (the rest of the index records) {
    Irec ir = addbtree(Irec(,), root); //returns an Irec
    if (ir.getblockid() != 0) {
      buildnewroot(ir);
    }
  }
}
*/


void FastTable::buildRoot(Irec r1, Irec r2) {
  if (r1.key > r2.key) {
    Irec temp = r1;
    r1 = r2;
    r2 = temp;
  }

  std::vector<std::string> k;
  std::vector<int> b;

  k.push_back("NU");
  k.push_back(r1.key);

  b.push_back(0);
  b.push_back(r1.block_id);

  Bnode left(k, b, DEFAULT_BLOCK_SIZE);

  // Empty the vectors
  k.clear();
  b.clear();

  k.push_back("NU");
  k.push_back(r2.key);

  b.push_back(0);
  b.push_back(r2.block_id);

  Bnode right(k, b, DEFAULT_BLOCK_SIZE);

  if (!addBlock(index_file, left.buffer)) return;
  int leftBlock = getFirstBlock(index_file);

  if (!addBlock(index_file, right.buffer)) return;
  int rightBlock = getNextBlock(index_file, leftBlock);

  k.clear();
  b.clear();

  k.push_back("NU");
  k.push_back(r1.key);

  b.push_back(leftBlock);
  b.push_back(rightBlock);

  Bnode rt(k, b, DEFAULT_BLOCK_SIZE);

  root_block = addBlock(index_file, rt.buffer);
}


int FastTable::indexSearch(std::string key) {
  return treeIndexSearch(root_block, key);
}


int FastTable::treeIndexSearch(int node,
                               std::string key) {
  std::string buffer;
  readBlock(index_file, node, buffer);
  Bnode c(buffer);
  std::vector<std::string> k = c.keys;
  std::vector<int> b = c.block_ids;

  if (b[0] == 0) {
    for (unsigned int i = 1; i < k.size(); ++i) {
      if (k[i] == key) {
        return b[i];
      }
    }
    return 0;
  } else {
    unsigned int i = 1;
    while (i < k.size() && key > k[i]) {
      ++i;
    }
    return treeIndexSearch(b[i - 1], key);
  }
}
