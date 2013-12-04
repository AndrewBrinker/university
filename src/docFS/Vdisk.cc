/*
 * Copyright 2013 Andrew Brinker
 */

#include <string>
#include <fstream>
#include <iostream>
#include "./Vdisk.h"

/*
 * Vdisk()
 *
 * @in: std::string current_name
 *   - The name of the disk to be loaded.
 * @return: none
 *
 * Load a disk, assuming one already exists.
 */
Vdisk::Vdisk(std::string current_name) {
  name = current_name;
  std::string arch_file = name + ".spc";
  std::ifstream file(arch_file.c_str());
  if (file.good()) {
    file >> block_count >> block_size;
  } else {
    std::cout << "The disk " << name << ".spc does not exist!" << std::endl;
  }
  file.close();
}


/*
 * Vdisk()
 *
 * @in: std::string new_name
 *   - The name of the disk being made
 * @in: unsigned int new_block_count
 *   - The number of blocks on the disk
 * @in: unsigned int new_block_size
 *   - The size of each block on the disk
 *
 * Create a new disk, assuming it doesn't already exist
 */
Vdisk::Vdisk(std::string new_name,
             unsigned int new_block_count,
             unsigned int new_block_size) {
  name = new_name;
  block_count = new_block_count;
  block_size = new_block_size;
  std::string arch_file_name = name + ".spc";
  std::ofstream arch_file(arch_file_name.c_str());
  arch_file << block_count << " " << block_size;
  arch_file.close();
  std::string data_file_name = name + ".dat";
  std::ofstream data_file(data_file_name.c_str());
  for (unsigned int i = 0; i < block_size * block_count; ++i) {
    data_file << "#";
  }
  data_file.close();
}


/*
 * getBlock()
 *
 * @in: unsigned int block_number
 *   - The block number data is being retrieved from
 * @out: std::string& block
 *   - The buffer the block contents are written out to
 * @return:
 *   - 1 if successful
 *   - 0 otherwise
 *
 * Retrieve data from the specified block and write it to the buffer.
 */
unsigned int Vdisk::getBlock(unsigned int block_number,
                             std::string& buffer) {
  std::string data_file = name + ".dat";
  std::ifstream file(data_file.c_str());
  if (file.bad()) { return 0; }
  file.seekg(block_number * block_size);
  for (unsigned int i = 0; i < block_size; ++i) {
    buffer += file.get();
  }
  return 1;
}


/*
 * putBlock()
 *
 * @in: unsigned int block_number
 *   - The block number being read from
 * @in: std::string buffer
 *   - The buffer being written to the block
 * @return:
 *   - 1 if successful
 *   - 0 otherwise
 *
 * Write the buffer to the block
 */
unsigned int Vdisk::putBlock(unsigned int block_number,
                             std::string buffer) {
  std::string data_file = name + ".dat";
  std::fstream file(data_file.c_str(), std::fstream::in | std::fstream::out);
  if (file.bad()) {
    return 0;
  }
  if (getFileSize(data_file) < (block_number + 1) * block_size - 1) {
    return 0;
  }
  if (buffer.length() != block_size) {
    return 0;
  }
  file.seekp(block_number * block_size);
  for (unsigned int i = 0; i < block_size; ++i) {
    file.put(buffer[i]);
  }
  file.close();
  return 1;
}


/*
 * getFileSize()
 *
 * @in: std::string filename
 *   - The name of the file being checked
 * @return:
 *   - The size of the file, in bytes
 *
 * Get the size of the given file in bytes.
 */
unsigned int Vdisk::getFileSize(std::string filename) {
  std::ifstream file(filename.c_str());
  file.seekg(0, file.end);
  unsigned int length = file.tellg();
  file.close();
  return length;
}
