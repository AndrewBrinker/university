/*
 * Copyright 2013 Andrew Brinker
 */

#ifndef SRC_DOCFS_TABLE_H_
#define SRC_DOCFS_TABLE_H_

#include <string>
#include "./FileSys.h"

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define INDEX_RECORD_LENGTH    10
#define DATE_LENGTH            5
#define END_LENGTH             5
#define TYPE_LENGTH            8
#define PLACE_LENGTH           15
#define REFERENCE_LENGTH       7
#define MAX_RECORD_LENGTH      120
#define MAX_DESCRIPTION_LENGTH (MAX_RECORD_LENGTH \
                               - DATE_LENGTH - END_LENGTH \
                               - TYPE_LENGTH - PLACE_LENGTH \
                               - REFERENCE_LENGTH )


class Table : public FileSys {
 public:
  Table(std::string, std::string, std::string);
  int buildTable(std::string);
  int search(std::string);

 private:
  int indexSearch(unsigned int, std::string);
  std::string flat_file;
  std::string index_file;

  DISALLOW_COPY_AND_ASSIGN(Table);
};

#endif  // SRC_DOCFS_TABLE_H_
