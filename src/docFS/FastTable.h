/*
 * Copyright 2013 Andrew Brinker
 */

#ifndef SRC_DOCFS_FASTTABLE_H_
#define SRC_DOCFS_FASTTABLE_H_

#include <string>
#include <vector>
#include "./Table.h"

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)


struct Bnode {
  Bnode(std::string);
  Bnode(std::vector<std::string>, std::vector<int>, int);
  std::string buffer;
  std::vector<std::string> keys;
  std::vector<int> block_ids;
};

typedef struct {
  std::string key;
  int block_id;
} Irec;


class FastTable : public Table {
 public:
  FastTable(std::string, std::string, std::string);
  int buildTable(std::string);
  int search(std::string);

 private:
  int indexSearch(std::string);
  int treeIndexSearch(int node, std::string);
  void buildRoot(Irec, Irec);

  int root_block;
  std::string flat_file;
  std::string index_file;

  DISALLOW_COPY_AND_ASSIGN(FastTable);
};

#endif  // SRC_DOCFS_FASTTABLE_H_
