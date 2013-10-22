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


// Checks if the file system has already been made. If it has, it loads the
// file system. If not, it makes a new one.
FileSystem::FileSystem(std::string new_name):VirtualDisk(new_name,
                                                         BLOCK_COUNT,
                                                         BLOCK_SIZE) {
  if (!loadFileSystem()) {
    makeFileSystem();
  }
}


// Writes out the current state of the root and the FAT to the disk.
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
    } else {
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


// Creates a new file with no allocated blocks. Returns 0 if the length of the
// filename is too big, if the file already exists, or if there is no more room
// in the root.
unsigned int FileSystem::newFile(std::string file) {
  const std::string default_file_name(MAX_NAME_LENGTH, FILL_CHAR);

  // If the length of the filename is wrong. Return 0.
  if (file.length() != MAX_NAME_LENGTH) {
    return 0;
  }

  // Iterate through root_file_names
  for (unsigned int i = 0; i < root_file_names.size(); ++i) {
    // If the filename is already present, return 0;
    if (file == root_file_names[i]) {
      return 0;
    }
  }

  // Iterate again
  for (unsigned int i = 0; i < root_file_names.size(); ++i) {
    // If the current filename is the default, put the new file here.
    if (default_file_name == root_file_names[i]) {
      root_file_names[i]   = file;
      root_first_blocks[i] = 0;

      sync();
      return 1;
    }
  }

  return 0;
}


// Deletes a file from the root. Returns 0 if the filename is the wrong length,
// or if the file is not empty, or if it's not in the root.
unsigned int FileSystem::removeFile(std::string file) {
  const std::string default_file_name(MAX_NAME_LENGTH, FILL_CHAR);

  // If the filename is the wrong length, return 0
  if (file.length() != MAX_NAME_LENGTH) {
    return 0;
  }

  // Iterate through the root
  for (unsigned int i = 0; i < root_file_names.size(); ++i) {
    // If the file is present...
    if (root_file_names[i] == file) {
      // If the file is not empty, return 0
      if (root_first_blocks[i] != 0) {
        return 0;
      }
      // Otherwise, delete it from the root.
      root_file_names[i] = default_file_name;

      sync();
      return 1;
    }
  }
  return 0;
}


// Returns the first block of a given file. Returns 0 if the file doesn't exist
// or is empty.
unsigned int FileSystem::getFirstBlock(std::string file) {
  // Go through the root. If the file exists, return its first block number
  for (unsigned int i = 0; i < root_file_names.size(); ++i) {
    if (file == root_file_names[i]) {
      return root_first_blocks[i];
    }
  }
  // Otherwise, return 0
  return 0;
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


unsigned int FileSystem::readBlock(std::string file,
                                   unsigned int block_number,
                                   std::string& buffer) {
  return 1;
}


unsigned int FileSystem::writeBlock(std::string file,
                                    unsigned int block_number,
                                    std::string buffer) {
  return 1;
}


// Load an existing file system
unsigned int FileSystem::loadFileSystem() {
  std::string fat_string = "";
  VirtualDisk::getBlock(2, fat_string);
  if (fat_string == std::string(BLOCK_SIZE, FILL_CHAR)) {
    return false;
  }
  std::string remaining_fat;
  for (unsigned int i = 1; i < FAT_BLOCK_COUNT; ++i) {
    VirtualDisk::getBlock(i, remaining_fat);
    fat_string += remaining_fat;
  }
  if (!loadFat(fat_string)) {
    return 0;
  }
  std::string root_string;
  VirtualDisk::getBlock(1, root_string);
  if (!loadRoot(root_string)) {
    return 0;
  }
  return 1;
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


// Load the FAT
unsigned int FileSystem::loadFat(std::string fat_string) {
  std::string raw_value = "";
  unsigned int current_value = 0;
  std::stringstream fat_stream;
  while(fat_string[0] != FILL_CHAR) {
    raw_value = fat_string.substr(0, ADDRESS_SPACE);
    fat_string.erase(0, ADDRESS_SPACE);
    strip(raw_value, ' ');
    fat_stream << raw_value;
    if (fat_stream >> current_value) {
      fat.push_back(current_value);
    } else {
      return 0;
    }
    fat_stream.clear();
  }
  return 1;
}


// Load the Root
unsigned int FileSystem::loadRoot(std::string root_string) {
  std::string raw_value = "";
  std::string filename = "";
  unsigned int address = 0;
  std::stringstream root_stream;
  while (root_string[0] != FILL_CHAR) {
    filename = root_string.substr(0, MAX_NAME_LENGTH);
    root_string.erase(0, MAX_NAME_LENGTH + 1);
    strip(filename, FILL_CHAR);
    raw_value = root_string.substr(0, ADDRESS_SPACE);
    root_string.erase(0, ADDRESS_SPACE);
    strip(raw_value, ' ');
    root_stream << raw_value;
    if (root_stream >> address) {
      root_file_names.push_back(filename);
      root_first_blocks.push_back(address);
    } else {
      return 0;
    }
    root_stream.clear();
  }
  return 1;
}

// Remove extra characters.
void FileSystem::strip(std::string& new_string, const char fill = FILL_CHAR) {
  while(new_string[0] == fill) {
    new_string.erase(0, 1);
  }
  while(new_string[new_string.length() - 1] == fill) {
    new_string.erase(new_string.length() - 1);
  }
}
