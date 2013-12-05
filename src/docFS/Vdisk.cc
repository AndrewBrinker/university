/*
 * Copyright 2013 Andrew Brinker
 */

#include <string>
#include <fstream>
#include <iostream>
#include "./Vdisk.h"

class CorruptDisk : public std::exception {
  const char* what() const throw() {
    return "Bad disk or disk spec";
  }
};


Vdisk::Vdisk(const std::string new_name)
           : Vdisk(new_name, DEFAULT_BLOCK_COUNT, DEFAULT_BLOCK_SIZE) {}


Vdisk::Vdisk(const std::string new_name,
             const unsigned int new_block_count,
             const unsigned int new_block_size) {
  name = new_name;
  block_count = new_block_count;
  block_size = new_block_size;

  std::fstream io_file;

  io_file.open((name + ".spc").c_str(), std::ios::in);

  if (io_file.good()) {
    io_file >> block_count >> block_size;

    if (io_file.fail()) {
      throw CorruptDisk();
    }
    io_file.close();

    io_file.open((name + ".dat").c_str(), std::ios::in);
    unsigned int count = 0;

    while (!io_file.eof() && !io_file.fail() && ++count) {
      io_file.get();
    }

    if (count != block_count * block_size + 1) {
      throw CorruptDisk();
    }
  } else {
    io_file.close();
    io_file.open((name + ".spc").c_str(), std::ios::out);
    io_file << block_count << ' ' << block_size;
    io_file.close();

    io_file.open((name + ".dat").c_str(), std::ios::out);

    for (unsigned int i = 0; i < block_count * block_size; ++i) {
      io_file.put(FILL_CHAR);
    }
    io_file.close();
  }
}


int Vdisk::getBlock(const unsigned int block_number,
                    std::string& buffer) const {
  std::ifstream input_file;
  int success = 0;
  input_file.open((name + ".dat").c_str());
  input_file.seekg(block_number * block_size);
  char* buf = new char[block_size + 1];
  input_file.get(buf, block_size + 1);
  buffer.assign(buf);
  if (input_file.good()) {
    success = 1;
  }
  delete[] buf;
  input_file.close();
  return success;
}


int Vdisk::putBlock(const unsigned int block_number,
                    const std::string buffer) const {
  std::fstream io_file;
  int success = 0;
  io_file.open((name + ".dat").c_str(), std::ios::out | std::ios::in);
  io_file.seekp(block_number * block_size);
  io_file.write(buffer.c_str(), block_size);
  if (io_file.good()) {
    success = 1;
  }
  io_file.close();
  return success;
}
