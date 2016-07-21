/*
 * Copyright 2013 Andrew Brinker
 */

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include "./FileSys.h"


FileSys::FileSys(std::string new_name) : Vdisk(new_name) {
  if (!loadFileSys()) makeFileSys();
}


int FileSys::sync() const {
  std::stringstream root_str, fat_str;
  std::string fat_partial;
  std::vector<std::string> fat_vector;

  // Build the root stringstream
  for (RootEntry entry : root) {
    root_str << entry.file_name
           << std::string(
                FILE_NAME_LENGTH - entry.file_name.length(),
                FILL_CHAR
              )
           << ' ' << std::setfill(' ')
           << std::setw(ADDRESS_LENGTH) << entry.start_block
           << ' ';
  }
  for (unsigned int i = root.size(); i < MAX_ROOT_SIZE; ++i) {
    root_str << std::string(FILE_NAME_LENGTH, FILL_CHAR)
           << ' '
           << std::setfill(' ')
           << std::setw(ADDRESS_LENGTH) << 0 << ' ';
  }

  while (root_str.str().length() < block_size) {
    root_str << FILL_CHAR;
  }

  // Place the root block
  if (!Vdisk::putBlock(ROOT_BLOCK, root_str.str())) {
      return 0;
  }

  // Create the fat stream
  for (unsigned int i : fat) {
    fat_str << std::setfill(' ') << std::setw(ADDRESS_LENGTH)
          << i << ' ';
  }

  // Separate the fat into blocks
  for (char c : fat_str.str()) {
    if (fat_partial.length() < block_size) {
      fat_partial.push_back(c);
    } else {
      fat_vector.push_back(fat_partial);
      fat_partial.clear();
    }
  }
  if (fat_partial.length() != 0) {
    for (unsigned int i = fat_partial.length(); i < block_size; ++i) {
      fat_partial.push_back(FILL_CHAR);
    }
    fat_vector.push_back(fat_partial);
  }

  // Place the fat blocks
  for (unsigned int i = 0; i < fat_vector.size(); ++i) {
    if (!Vdisk::putBlock(FAT_BLOCK + i, fat_partial)) {
      return 0;
    }
  }
  return 1;
}


int FileSys::makeFile(const std::string file_name) {
  if (root.size() >= MAX_ROOT_SIZE) return 0;
  if (fat[0] == 0) return 0;
  if (root.find({file_name}) != root.end()) return 0;

  root.insert({file_name, 0});
  return sync();
}


int FileSys::removeFile(const std::string file_name) {
  std::set<RootEntry>::const_iterator it = root.find({file_name});
  if (it == root.end()) return 0;
  if (it->start_block != 0) return 0;

  fat[it->start_block] = fat[0];
  root.erase(it);
  return sync();
}


unsigned int FileSys::getFirstBlock(const std::string file_name) const {
  std::set<RootEntry>::const_iterator it = root.find({file_name});
  if (it == root.end()) return -1;
  return it->start_block;
}


int FileSys::addBlock(const std::string file_name, const std::string buffer) {
  std::set<RootEntry>::const_iterator it = root.find({file_name});

  if (it == root.end()) return 0;
  if (fat[0] == 0) return -1;

  unsigned int block = fat[0];
  unsigned int current_block = it->start_block;

  fat[0] = fat[block];
  fat[block] = 0;

  if (current_block != 0) {
    while (fat[current_block] != 0) {
      current_block = fat[current_block];
    }
    fat[current_block] = block;
  } else {
    root.erase(it);
    root.insert({file_name, block});
  }

  Vdisk::putBlock(block, buffer);
  return sync();
}


int FileSys::deleteBlock(const std::string file_name,
                         const unsigned int input_block) {
  std::set<RootEntry>::const_iterator it = root.find({file_name});
  if (it == root.end()) return 0;

  if (it->start_block == input_block) {
    root.erase(it);
    root.insert({file_name, fat[input_block]});
    fat[input_block] = fat[0];
    fat[0] = input_block;
    return 1;
  }

  unsigned int previous_block = it->start_block;
  while (fat[previous_block] != input_block) {
    if (fat[previous_block] == END) {
      return 0;
    }
    previous_block = fat[previous_block];
  }
  fat[previous_block] = fat[input_block];
  fat[input_block] = fat[0];
  fat[0] = input_block;

  return sync();
}


int FileSys::readBlock(const std::string file_name,
                       const unsigned int input_block,
                       std::string& buffer) const {
  std::set<RootEntry>::const_iterator it = root.find({file_name});
  if (it == root.end()) return 0;
  unsigned int block = it->start_block;
  while (block != input_block) {
    if (block == END) {
      return 0;
    }
    block = fat[block];
  }
  return Vdisk::getBlock(input_block, buffer);
}


int FileSys::writeBlock(const std::string file_name,
                        const unsigned int input_block,
                        const std::string buffer) const {
  std::set<RootEntry>::const_iterator it = root.find({file_name});
  if (it == root.end()) {
    return 0;
  }
  unsigned int block = it->start_block;
  while (block != input_block) {
    if (block == END) {
      return 0;
    }
    block = fat[block];
  }
  return Vdisk::putBlock(input_block, buffer);
}


int FileSys::getNextBlock(const std::string file_name,
                          const unsigned int input_block) const {
  std::set<RootEntry>::const_iterator it = root.find({file_name});
  if (it == root.end()) return -1;
  unsigned int block = it->start_block;
  while (block != input_block) {
    if (block == END) {
      return 0;
    }
    block = fat[block];
  }
  return fat[block];
}


bool FileSys::loadFileSys() {
  std::string sfat;
  Vdisk::getBlock(FAT_BLOCK, sfat);
  if (sfat == std::string(block_size, FILL_CHAR)) return false;
  std::string morefat;
  for (unsigned int i = 1; i < FAT_SIZE; ++i) {
    Vdisk::getBlock(i, morefat);
    sfat += morefat;
  }
  if (!loadFAT(sfat)) return false;
  std::string sroot;
  Vdisk::getBlock(ROOT_BLOCK, sroot);
  if (!loadRoot(sroot)) return false;
  return true;
}


void FileSys::makeFileSys() {
  fat.resize(block_count);
  fat[0] = 2 + FAT_SIZE;

  for (unsigned int i = 1; i < fat[0]; ++i) {
    fat[i] = RESERVED;
  }

  for (unsigned int i = fat[0]; i < fat.size() - 1; ++i) {
    fat[i] = i + 1;
  }
  fat[fat.size() - 1] = END;
  sync();
}


bool FileSys::loadFAT(std::string sfat) {
  std::string raw;
  int i;
  std::stringstream ss;
  while (sfat[0] != FILL_CHAR) {
    raw = sfat.substr(0, ADDRESS_SPACE);
    sfat.erase(0, ADDRESS_SPACE);
    strip(raw, ' ');
    ss << raw;
    if (ss >> i) {
      fat.push_back(i);
    } else {
      return false;
    }
    ss.clear();
  }
  return true;
}


bool FileSys::loadRoot(std::string sroot) {
  std::string raw, file_name;
  unsigned int address;
  std::stringstream ss;

  while (sroot[0] != FILL_CHAR) {
    file_name = sroot.substr(0, FILE_NAME_LENGTH);
    sroot.erase(0, FILE_NAME_LENGTH + 1);
    strip(file_name, FILL_CHAR);
    raw = sroot.substr(0, ADDRESS_SPACE);
    sroot.erase(0, ADDRESS_SPACE);
    strip(raw, ' ');
    ss << raw;
    if (ss >> address) {
      root.insert({file_name, address});
    } else {
      return false;
    }
    ss.clear();
  }
  return true;
}


void FileSys::strip(std::string& str, const char c = FILL_CHAR) {
  while (str.front() == c) str.erase(0, 1);
  while (str.back() == c) str.erase(str.length() - 1);
}
