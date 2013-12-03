// This is the interface for the VirtualDisk class, which simulates a physical
// disk by reading and writing to a data file. It also has an architecture file,
// which contains information about the disk itself for persistence across
// initializations of the class.
// There are two constructors. The first just takes a disk name, and it used
// only when the disk is already present. If the disk does not exist the
// initialization fails. The second takes all the information needed to make a
// new disk. If there is already a disk present it destroys the disk and makes a
// new one. BE VERY CAREFUL ABOUT OVERWRITING EXISTING DISKS.

#ifndef DOCFS_SRC_VDISK_H
#define DOCFS_SRC_VDISK_H

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


#endif  // DOCFS_SRC_VDISK_H
