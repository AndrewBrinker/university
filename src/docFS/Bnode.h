/*
 * Copyright 2013 Andrew Brinker
 */

#ifndef SRC_DOCFS_BNODE_H_
#define SRC_DOCFS_BNODE_H_

#include <string>
#include <vector>

class Bnode {
 public:
  explicit Bnode(std::string);
  Bnode(std::vector<std::string>, std::vector<unsigned int>, unsigned int);
  inline std::string getBuffer() { return buffer; }
  inline std::vector<std::string> getKeys() { return keys; }
  inline std::vector<unsigned int> getBlockIDs() { return block_ids; }

 private:
  std::string buffer;
  std::vector<std::string> keys;
  std::vector<unsigned int> block_ids;
};

#endif  // SRC_DOCFS_BNODE_H_
