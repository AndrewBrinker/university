/*
 * Copyright 2013 Andrew Brinker
 */

#ifndef SRC_DOCFS_BTREETABLE_H_
#define SRC_DOCFS_BTREETABLE_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#include <string>
#include <vector>
#include "./FileSys.h"

class BTreeTable : public FileSys {
 public:
  BTreeTable(std::string new_diskname,
             std::string new_flat_file,
             std::string new_index_file);
  unsigned int buildTable(std::string input_file);
  void buildRoot(Irec r1, Irec r2);
  unsigned int addIndexRecord(Bnode node, Irec record);
  unsigned int search(std::string value);
 private:
  unsigned int indexSearch(std::string value);
  int root;
  std::string flat_file;
  std::string index_file;

  DISALLOW_COPY_AND_ASSIGN(BTreeTable);
};

#endif  // SRC_DOCFS_BTREETABLE_H_
