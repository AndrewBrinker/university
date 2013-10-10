/*
The MIT License (MIT)

Copyright (c) 2013 Andrew Brinker

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

// This is the implementation of the class FileSystem. It implements all
// non-inline functions of the class.

#include "./FileSystem.h"
#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

// Defined defaults.
#define BLOCK_SIZE       500
#define BLOCK_COUNT      100
#define MAX_NAME_LENGTH  16
#define FILL_CHAR        '#'
#define EOF_CHAR         0
#define RESERVED_CHAR    1

#define ROOT_BLOCK      1
#define FAT_START_BLOCK 2

// Calculated values.
#define ADDRESS_LENGTH   (floor(log10(BLOCK_COUNT)))
#define ADDRESS_SPACE    (ADDRESS_LENGTH+1)
#define ROOT_ENTRY_SPACE (MAX_NAME_LENGTH+ADDRESS_LENGTH+2)
#define ROOT_ENTRY_COUNT (BLOCK_SIZE/ROOT_ENTRY_SPACE)
#define FAT_BLOCK_COUNT  ((ADDRESS_SPACE*BLOCK_COUNT)/BLOCK_SIZE+1)


FileSystem::FileSystem(std::string new_name):VirtualDisk(new_name,
                                                         BLOCK_COUNT,
                                                         BLOCK_SIZE) {
  if (!loadFileSystem()) {
    makeFileSystem();
  }
}


unsigned int FileSystem::sync() {

  std::stringstream root_stream;
  std::stringstream fat_stream;
  std::vector<std::string> fat_vector;
  std::string partial_fat;

  // Building the root stream
  for (unsigned int i = 0; i < root_file_names.size(); ++i) {
    root_stream << root_file_names[i]
                << std::string(MAX_NAME_LENGTH - root_file_names[i].length(),
                               FILL_CHAR)
                << " " << std::setfill(' ') << std::setw(ADDRESS_LENGTH)
                << root_first_blocks[i] << " ";
  }

  // Pad the root size
  while (root_stream.str().length() < BLOCK_SIZE) {
      root_stream << FILL_CHAR;
  }

  // Place the root at ROOT_BLOCK. If it fails return an error code
  if (!VirtualDisk::putBlock(ROOT_BLOCK, root_stream.str())) {
    return 0;
  }

  // Build the FAT stream
  for (unsigned int i = 0; i < fat.size(); ++i) {
    fat_stream << std::setfill(' ') << std::setw(ADDRESS_LENGTH)
               << fat[i] << ' ';
  }

  // Separate the FAT into blocks
  for (unsigned int i = 0; i < fat_stream.str().size(); ++i) {
    char c = fat_stream.str()[i];
    if (partial_fat.length() < BLOCK_SIZE) {
      partial_fat.push_back(c);
    }
    else {
      fat_vector.push_back(partial_fat);
      partial_fat.clear();
    }
  }

  if (partial_fat.length() != 0) {
    // Make sure to add the last block and pad it if it's not the right size.
    for (unsigned int i = partial_fat.length(); i < BLOCK_SIZE; ++i) {
      partial_fat.push_back(FILL_CHAR);
    }
    fat_vector.push_back(partial_fat);
  }

  // Output each block individually
  for (unsigned int i = 0; i < fat_vector.size(); ++i) {
    if (!VirtualDisk::putBlock(FAT_START_BLOCK + i, partial_fat)) {
      return 0;
    }
  }

  return 1;
}


unsigned int FileSystem::newFile(std::string file) {
  return 1;
}


unsigned int FileSystem::removeFile(std::string file) {
  return 1;
}


unsigned int FileSystem::getFirstBlock(std::string file) {
  return 1;
}


unsigned int FileSystem::nextBlock(std::string file,
                                   unsigned int block_number) {
  return 1;
}


unsigned int FileSystem::addBlock(std::string file,
                                  std::string block) {
  return 1;
}


unsigned int FileSystem::deleteBlock(std::string file,
                                     unsigned int block_number) {
  return 1;
}


unsigned int FileSystem::getBlock(std::string file,
                                  unsigned int block_number,
                                  std::string& buffer) {
  return 1;
}


unsigned int FileSystem::putBlock(std::string file,
                                  unsigned int block_number,
                                  std::string buffer) {
  return 1;
}


// Load an existing file system
unsigned int FileSystem::loadFileSystem() {
  return 0;
}


// Make a new file system
unsigned int FileSystem::makeFileSystem() {
  const std::string default_file_name(MAX_NAME_LENGTH, FILL_CHAR);

  // Because we already know the size, it's more efficient to resize now
  // instead of using push_back over and over, which would potentially result
  // in several resizes.
  root_file_names.resize(ROOT_ENTRY_COUNT);
  root_first_blocks.resize(ROOT_ENTRY_COUNT);

  for (unsigned int i = 0; i < ROOT_ENTRY_COUNT; ++i) {
    root_file_names[i] = default_file_name;
    root_first_blocks[i] = EOF_CHAR;
  }

  // Next we construct the FAT.
  fat.resize(BLOCK_COUNT);

  fat[0] = FAT_BLOCK_COUNT + 2;
  for (unsigned int i = 1; i < FAT_BLOCK_COUNT + 2; ++i) {
    fat[i] = RESERVED_CHAR;
  }
  for (unsigned int i = FAT_BLOCK_COUNT + 2; i < fat.size(); ++i) {
    fat[i] = i + 1;
  }
  fat[fat.size() - 1] = EOF_CHAR;

  return sync();
}
