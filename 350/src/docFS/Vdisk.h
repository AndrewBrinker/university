/*
 * Copyright 2013 Andrew Brinker
 */

#ifndef SRC_DOCFS_VDISK_H_
#define SRC_DOCFS_VDISK_H_

#include <string>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define DEFAULT_BLOCK_COUNT 100
#define DEFAULT_BLOCK_SIZE  500
#define FILL_CHAR           '#'

class Vdisk {
 public:
  explicit Vdisk(const std::string new_name);
  Vdisk(const std::string, const unsigned int, const unsigned int);
  int getBlock(const unsigned int, std::string&) const;
  int putBlock(const unsigned int, const std::string) const;

  inline std::string getName() const { return name; }
  inline unsigned int getBlockCount() const { return block_count; }
  inline unsigned int getBlockSize() const { return block_size; }

 protected:
  std::string name;          // Filename of software disk
  unsigned int block_count;  // Number of blocks on disk
  unsigned int block_size;   // Block size in bytes

 private:
  DISALLOW_COPY_AND_ASSIGN(Vdisk);
};

#endif  // SRC_DOCFS_VDISK_H_
