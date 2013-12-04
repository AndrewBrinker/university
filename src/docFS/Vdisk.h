/*
 * Copyright 2013 Andrew Brinker
 */

#ifndef SRC_DOCFS_VDISK_H_
#define SRC_DOCFS_VDISK_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#include <string>


class Vdisk {
 public:
  explicit Vdisk(std::string current_name);
  Vdisk(std::string new_name,
        unsigned int new_block_count,
        unsigned int new_block_size);
  unsigned int getBlock(unsigned int block_number, std::string& buffer);
  unsigned int putBlock(unsigned int block_number, std::string buffer);

  inline std::string getName() { return name; }
  inline unsigned int getBlockCount() { return block_count; }
  inline unsigned int getBlockSize() { return block_size; }
 private:
  unsigned int getFileSize(std::string filename);
  std::string name;          // Filename of software disk
  unsigned int block_count;  // Number of blocks on disk
  unsigned int block_size;   // Block size in bytes

  DISALLOW_COPY_AND_ASSIGN(Vdisk);
};

#endif  // SRC_DOCFS_VDISK_H_
