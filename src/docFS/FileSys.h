/*
 * Copyright 2013 Andrew Brinker
 */

#ifndef SRC_DOCFS_FILESYS_H_
#define SRC_DOCFS_FILESYS_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include "./Vdisk.h"

#define FILE_NAME_LENGTH   16
#define END                0
#define RESERVED           1
#define DEFAULT_FILE_NAME  std::string(FILE_NAME_LENGTH, FILL_CHAR)

#define ADDRESS_LENGTH     intlog(block_count - 1, 10)
#define ADDRESS_SPACE      (ADDRESS_LENGTH + 1)
#define FAT_SIZE           ((ADDRESS_SPACE * block_count) / block_size + 1)
#define MAX_ROOT_SIZE      (block_size / (FILE_NAME_LENGTH + 1 + ADDRESS_SPACE))
#define DEFAULT_ROOT_ENTRY RootEntry(DEFAULT_FILE_NAME, 0)

#define ROOT_BLOCK         1
#define FAT_BLOCK          2

constexpr unsigned int intlog(const unsigned int n,
                              const unsigned int b = 10,
                              const unsigned int i = 1) {
    return (n / b == 0) ? i : intlog(n / b, b, i + 1);
}


struct RootEntry {
  RootEntry()
          : RootEntry(DEFAULT_FILE_NAME, 0) {}
  RootEntry(std::string f)
          : RootEntry(f, 0) {}
  RootEntry(std::string f, unsigned int s)
          : file_name(f), start_block(s) {}

  std::string file_name = DEFAULT_FILE_NAME;
  unsigned int start_block = 0;

  bool empty() const {
    return (file_name == DEFAULT_FILE_NAME);
  }

  bool operator < (const RootEntry& other) const {
    return (other.file_name == DEFAULT_FILE_NAME) ?
      true :
      file_name < other.file_name;
  }
};


class FileSys : public Vdisk {
 public:
  explicit FileSys(std::string disk_name);
  int sync() const;
  int makeFile(const std::string);
  int removeFile(const std::string);
  int addBlock(const std::string, const std::string);
  int deleteBlock(const std::string, const unsigned int);
  int readBlock(const std::string,
                const unsigned int,
                std::string&) const;
  int writeBlock(const std::string,
                 const unsigned int,
                 const std::string) const;
  int getNextBlock(const std::string, const unsigned int) const;
  unsigned int getFirstBlock(const std::string) const;

 protected:
  bool loadFileSys();
  void makeFileSys();

  std::set<RootEntry> root;
  std::vector<unsigned int> fat;

 private:
  bool loadFAT(std::string);
  bool loadRoot(std::string);
  static void strip(std::string&, char);

  DISALLOW_COPY_AND_ASSIGN(FileSys);
};


#endif  // SRC_DOCFS_FILESYS_H_
