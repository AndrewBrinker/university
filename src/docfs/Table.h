/*
 * Copyright 2013 Andrew Brinker
 */

#ifndef SRC_DOCFS_TABLE_H_
#define SRC_DOCFS_TABLE_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#include <string>
#include <vector>
#include "./FileSys.h"

class Table : public FileSys {
 public:
  Table(std::string new_diskname,
        std::string new_flat_file,
        std::string new_index_file);
  unsigned int buildTable(std::string input_file);
  unsigned int search(std::string value);
 private:
  unsigned int indexSearch(std::string value);
  std::string flat_file;
  std::string index_file;

  DISALLOW_COPY_AND_ASSIGN(Table);
};

#endif  // SRC_DOCFS_TABLE_H_
