/*
 * Copyright 2013 Andrew Brinker
 */

#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include "./FileSys.h"

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


/*
 * FileSys()
 *
 * @in: std::string new_name
 *   - The name of the file system being created
 * @return: none
 *
 * Try to load the file system, if it fails, make a new one.
 */
FileSys::FileSys(std::string new_name):Vdisk(new_name,
                                             BLOCK_COUNT,
                                             BLOCK_SIZE) {
  if (!loadFileSystem()) {
    makeFileSystem();
  }
}


/*
 * sync()
 *
 * @return:
 *   - 1 if successful
 *   - 0 otherwise
 *
 * Write the Root and FAT to the disk.
 */
unsigned int FileSys::sync() {
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
  if (!Vdisk::putBlock(ROOT_BLOCK, root_stream.str())) {
    return 0;
  }

  // Build the FAT stream
  for (unsigned int i = 0; i < fat.size(); ++i) {
    fat_stream << std::setfill(' ') << std::setw(ADDRESS_LENGTH)
               << fat[i] << ' ';
  }

  // Separate the FAT into blocks
  fat_vector = block(fat_stream.str());

  // Output each block individually
  for (unsigned int i = 0; i < fat_vector.size(); ++i) {
    if (!Vdisk::putBlock(FAT_START_BLOCK + i, fat_vector[i])) {
      return 0;
    }
  }
  return 1;
}


/*
 * newFile()
 *
 * @in: std::string file
 *   - The name of the file being created.
 * @return:
 *   - sync() if successful
 *   - 0 otherwise
 *
 * Creates a new file and syncs the changes to the disk.
 */
unsigned int FileSys::newFile(std::string file) {
  const std::string default_file_name(MAX_NAME_LENGTH, FILL_CHAR);

  // If the length of the filename is wrong. Return 0.
  if (file.length() > MAX_NAME_LENGTH) {
    return 0;
  } else if (file.length() < MAX_NAME_LENGTH) {
    for (unsigned int i = file.length(); i < MAX_NAME_LENGTH; ++i) {
      file += FILL_CHAR;
    }
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
      return sync();
    }
  }
  return 0;
}


/*
 * removeFile()
 *
 * @in: std::string file
 *   - The name of the file being deleted.
 * @return:
 *   - sync() if successful
 *   - 0 otherwise
 *
 * Delete the file and sync the changes to the disk.
 */
unsigned int FileSys::removeFile(std::string file) {
  const std::string default_file_name(MAX_NAME_LENGTH, FILL_CHAR);

  // If the filename is the wrong length, return 0
  if (file.length() > MAX_NAME_LENGTH) {
    return 0;
  } else if (file.length() < MAX_NAME_LENGTH) {
    for (unsigned int i = file.length(); i < MAX_NAME_LENGTH; ++i) {
      file += FILL_CHAR;
    }
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
      return sync();
    }
  }
  return 0;
}


/*
 * getFirstBlock()
 *
 * @in: std::string file
 *   - The name of the file the first block is being retrieved from.
 * @return:
 *   - The block number or 0.
 *
 * Gets the first block of the given file.
 */
unsigned int FileSys::getFirstBlock(std::string file) {
  prepFileName(file);
  // Go through the root. If the file exists, return its first block number
  for (unsigned int i = 0; i < root_file_names.size(); ++i) {
    if (file == root_file_names[i]) {
      return root_first_blocks[i];
    }
  }
  // Otherwise, return 0
  return 0;
}


/*
 * getNextBlock()
 *
 * @in: std::string file
 *   - The name of the file being searched through
 * @in: unsigned int block_number
 *   - The number of the current block in the given file
 * @return:
 *   - next block if successful
 *   - -1 if the block doesn't exist, or has no blocks
 *
 * Gets the block after the block_number block.
 */
int FileSys::getNextBlock(std::string file, unsigned int block_number) {
  prepFileName(file);
  unsigned int current_block = 0;
  int block_found = 0;
  for (unsigned int i = 0; i < root_file_names.size(); ++i) {
    if (file == root_file_names[i]) {
      current_block = root_first_blocks[i];
      block_found = 1;
      break;
    }
  }
  // If the file doesn't exist, or has no blocks, return -1.
  if (block_found == 0 || current_block == 0) {
    return -1;
  }
  // Then go from block to block. If any of the blocks matches block_number,
  // return the next block. If there are no blocks after block_number,
  // return 0. If block_number never occurs, return -1.
  while (current_block != 0) {
    if (current_block == block_number) {
      return fat[current_block];
    }
    current_block = fat[current_block];
  }
  return -1;
}


/*
 * addBlock()
 *
 * @in: std::string file
 *   - The file being added to
 * @in: std::string buffer
 *   - The buffer containing the data being added to the file.
 * @return:
 *   - 1 if successful
 *   - 0 otherwise
 *
 * Adds a block to the given file, and writes the buffer to the block.
 */
int FileSys::addBlock(std::string file,
                      std::string buffer) {
  prepFileName(file);
  if (fat[0] == 0) {
    return -1;
  }
  unsigned int file_exists = 0;
  unsigned int file_index = 0;
  for (unsigned int i = 0; i < root_file_names.size(); ++i) {
    if (file == root_file_names[i]) {
      file_exists = 1;
      file_index = i;
    }
  }
  if (file_exists == 0) {
    return 0;
  }
  unsigned int first_free = fat[0];
  unsigned int current_block = root_first_blocks[file_index];
  fat[0] = fat[first_free];
  fat[first_free] = 0;
  while (fat[current_block] != 0) {
    current_block = fat[current_block];
  }
  fat[current_block] = first_free;
  Vdisk::putBlock(first_free, buffer);
  return sync();
}


/*
 * deleteBlock()
 *
 * @in: std::string file
 *   - The file being deleted from
 * @in: unsigned int block_number
 *   - The block number being deleted
 * @return:
 *   - sync if successful
 *   - 0 otherwise
 */
unsigned int FileSys::deleteBlock(std::string file,
                                  unsigned int block_number) {
  prepFileName(file);
  // Go through the file blocks.
  // If you hit the block you're looking for, set the previous block equal to
  // the value of the block you're deleting, and update the free list by setting
  // the deleted block equal to the value of fat[0], and set fat[0] equal to the
  // index of the deleted block.
  unsigned int block_index;
  for (unsigned int i = 0; i < root_file_names.size(); ++i) {
    if (file == root_file_names[i]) {
      block_index = i;
    }
  }
  unsigned int current_block = block_index;
  while (current_block != 0) {
    if (current_block == block_number) {
      unsigned int temp = current_block;
      current_block = fat[0];
      fat[0] = temp;
      return sync();
    }
    current_block = fat[current_block];
  }
  return 0;
}


/*
 * readBlock()
 *
 * @in: std::string file
 *   - The file being read from
 * @in: unsigned int block_number
 *   - The block number in the file being read from
 * @output: std::string& buffer
 *   - The buffer being written to
 * @return:
 *   - 1 if successful
 *   - 0 otherwise
 *
 * Reads from the given block.
 */
unsigned int FileSys::readBlock(std::string file,
                                unsigned int block_number,
                                std::string& buffer) {
  prepFileName(file);
  if (fileHasBlock(file, block_number)) {
    return Vdisk::getBlock(block_number, buffer);
  }
  return 0;
}


/*
 * writeBlock()
 *
 * @in: std::string file
 *   - The file being written to
 * @in: unsigned int block_number
 *   - The block number being written to
 * @in: std::string buffer
 *   - The contents being written to the given block
 * @return:
 *   - 1 if successful
 *   - 0 otherwise
 *
 * Writes the buffer to the given block.
 */
unsigned int FileSys::writeBlock(std::string file,
                                    unsigned int block_number,
                                    std::string buffer) {
  prepFileName(file);
  if (fileHasBlock(file, block_number)) {
    return Vdisk::putBlock(block_number, buffer);
  }
  return 0;
}


/*
 * loadFileSystem()
 *
 * @return:
 *   - 1 if successful
 *   - 0 otherwise
 *
 * Attempts to load the given file system. If the system doesn't exist, it
 * returns 0.
 */
unsigned int FileSys::loadFileSystem() {
  std::string fat_string = "";
  // Get the contents of the first FAT block
  Vdisk::getBlock(2, fat_string);
  // If they are just filler, return 0.
  if (fat_string == std::string(BLOCK_SIZE, FILL_CHAR)) {
    return 0;
  }
  // Otherwise, get the rest of the FAT.
  std::string remaining_fat;
  for (unsigned int i = 1; i < FAT_BLOCK_COUNT; ++i) {
    Vdisk::getBlock(i, remaining_fat);
    fat_string += remaining_fat;
  }
  if (!loadFat(fat_string)) {
    return 0;
  }
  std::string root_string;
  Vdisk::getBlock(1, root_string);
  if (!loadRoot(root_string)) {
    return 0;
  }
  return 1;
}


/*
 * makeFileSystem()
 *
 * @return:
 *   - 1 if successful
 *   - 0 otherwise
 *
 * Creates a new file system based on the macros set at the top of the file.
 */
unsigned int FileSys::makeFileSystem() {
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


/*
 * loadFat()
 *
 * @in: std::string fat_string
 *   - The string containing the FAT
 * @return:
 *   - 1 if successful
 *   - 0 otherwise
 *
 * Attempts to load and store the FAT in the disk.
 */
unsigned int FileSys::loadFat(std::string fat_string) {
  std::string raw_value = "";
  unsigned int current_value = 0;
  std::stringstream fat_stream;
  while (fat_string[0] != FILL_CHAR) {
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


/*
 * loadRoot()
 *
 * @in: std::string root_string
 *   - The contents of the Root
 * @return:
 *   - 1 if successful
 *   - 0 otherwise
 *
 * Attempts to load and store the Root in the disk.
 */
unsigned int FileSys::loadRoot(std::string root_string) {
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


/*
 * fileHasBlock()
 *
 * @in: std::string filename
 *   - The file being checked
 * @in: unsigned int block_number
 *   - The block being checked
 * @return:
 *   - 1 if successful
 *   - 0 otherwise
 *
 * Checks whether the given file has the given block.
 */
unsigned int FileSys::fileHasBlock(std::string filename,
                                      unsigned int block_number) {
  prepFileName(filename);
  unsigned int file_exists = 0;
  unsigned int file_index = 0;
  for (unsigned int i = 0; i < root_file_names.size(); ++i) {
    if (filename == root_file_names[i]) {
      file_exists = 1;
      file_index = i;
    }
  }
  if (file_exists == 0) {
    return 0;
  }
  unsigned int current_block = root_first_blocks[file_index];
  while (current_block != 0) {
    if (block_number == current_block) {
      return 1;
    }
    current_block = fat[current_block];
  }
  return 0;
}


/*
 * prepFileName()
 *
 * @inout: std::string& file
 *   - The filename being modified
 * @return:
 *   - 1 if successful
 *   - 0 otherwise
 *
 * Modifies the given file name to fit the specifications of the disk.
 */
unsigned int FileSys::prepFileName(std::string& file) {
  if (file.length() >= MAX_NAME_LENGTH) {
    return 0;
  }
  for (unsigned int i = file.length(); i < MAX_NAME_LENGTH; ++i) {
    file += FILL_CHAR;
  }
  return 1;
}


/*
 * strip()
 *
 * @inout: std::string& new_string
 *   - The string being stripped
 * @in: const char fill = FILL_CHAR
 *   - The fill character being stripped to the new string
 * @return: none
 *
 * Strips excess characters from the front of the string.
 */
void FileSys::strip(std::string& new_string, const char fill = FILL_CHAR) {
  while (new_string[0] == fill) {
    new_string.erase(0, 1);
  }
  while (new_string[new_string.length() - 1] == fill) {
    new_string.erase(new_string.length() - 1);
  }
}


/*
 * block()
 *
 * @in: std::string blocks
 *   - The string to be partitioned
 * @return:
 *   - The partitioned string
 *
 * Partitions the given string into block-sized increments.
 */
std::vector<std::string> FileSys::block(std::string blocks) {
  std::string partial;
  std::vector<std::string> result;
  for (unsigned int i = 0; i < blocks.length(); ++i) {
    char c = blocks[i];
    if (partial.length() < BLOCK_SIZE) {
      partial.push_back(c);
    } else {
      result.push_back(partial);
      partial.clear();
    }
  }
  if (partial.length() != 0) {
    // Make sure to add the last block and pad it if it's not the right size.
    for (unsigned int i = partial.length(); i < BLOCK_SIZE; ++i) {
      partial.push_back('#');
    }
    result.push_back(partial);
    partial.clear();
  }
  return result;
}
