/*
 * Copyright 2013 Andrew Brinker
 */

#ifndef SRC_DOCFS_FASTTABLE_H_
#define SRC_DOCFS_FASTTABLE_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#include <string>
#include <vector>
#include "./FileSys.h"
#include "./Bnode.h"
#include "./Irec.h"

class FastTable : public FileSys {
 public:
  FastTable(std::string, std::string, std::string);
  void buildRoot(Irec, Irec);
  unsigned int addIndexRecord(Bnode, Irec);
  unsigned int search(std::string);
 private:
  unsigned int indexSearch(unsigned int node, std::string);

  int root_block;
  std::string flat_file;
  std::string index_file;

  DISALLOW_COPY_AND_ASSIGN(FastTable);
};

#endif  // SRC_DOCFS_FASTTABLE_H_
