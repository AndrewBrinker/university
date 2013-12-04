/*
 * Copyright 2013 Andrew Brinker
 */

#ifndef SRC_DOCFS_BNODE_H_
#define SRC_DOCFS_BNODE_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#include <string>
#include <vector>

class Bnode {
 public:
  explicit Bnode(std::string new_buffer);
  Bnode(std::vector<std::string> new_key,
        std::vector<unsigned int> new_block_id,
        unsigned int new_block_size);
  inline std::string getBuffer() { return buffer; }
  inline std::vector<std::string> getKeys() { return key; }
  inline std::vector<unsigned int> getBlockIDs() { return block_id; }

 private:
  std::string buffer;
  std::vector<std::string> key;
  std::vector<unsigned int> block_id;

  DISALLOW_COPY_AND_ASSIGN(Bnode);
};

#endif  // SRC_DOCFS_BNODE_H_
