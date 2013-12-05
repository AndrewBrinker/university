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
  BTreeTable(std::string,
             std::string,
             std::string);
  unsigned int buildTable(std::string input_file);
  void buildRoot(Irec, Irec);
  unsigned int addIndexRecord(Bnode, Irec);
  unsigned int search(std::string);
 private:
  unsigned int indexSearch(std::string);

  int root_block;
  std::string flat_file;
  std::string index_file;

  DISALLOW_COPY_AND_ASSIGN(FastTable);
};

#endif  // SRC_DOCFS_FASTTABLE_H_
