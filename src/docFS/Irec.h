/*
 * Copyright 2013 Andrew Brinker
 */

#ifndef SRC_DOCFS_IREC_H_
#define SRC_DOCFS_IREC_H_

#include <string>

class Irec {
 public:
  Irec(std::string, unsigned int);
  inline std::string getKey() { return key; }
  inline unsigned int getBlockID() { return block_id; }

 private:
  std::string key;
  unsigned int block_id;
};

#endif  // SRC_DOCFS_IREC_H_
