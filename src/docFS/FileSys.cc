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
  std::stringstream ssroot, ssfat;
  std::string fat_part;
  std::vector<std::string> fat_vector;

  // Build the root stringstream
  for (RootEntry entry : root) {
    ssroot << entry.filename
           << std::string(
                FILE_NAME_LENGTH - entry.filename.length(),
                FILL_CHAR
              )
           << ' ' << std::setfill(' ')
           << std::setw(ADDRESS_LENGTH) << entry.startblock
           << ' ';
  }
  for (unsigned int i = root.size(); i < MAX_ROOT_SIZE; ++i) {
    ssroot << std::string(FILE_NAME_LENGTH, FILL_CHAR)
           << ' '
           << std::setfill(' ')
           << std::setw(ADDRESS_LENGTH) << 0 << ' ';
  }

  while (ssroot.str().length() < block_size) {
    ssroot << FILL_CHAR;
  }

  // Place the root block
  if (!Vdisk::putBlock(ROOT_BLOCK, ssroot.str())) {
      return 0;
  }

  // Create the fat stream
  for (unsigned int i : fat) {
    ssfat << std::setfill(' ') << std::setw(ADDRESS_LENGTH)
          << i << ' ';
  }

  // Separate the fat into blocks
  for (char c : ssfat.str()) {
    if (fat_part.length() < block_size) {
      fat_part.push_back(c);
    } else {
      fat_vector.push_back(fat_part);
      fat_part.clear();
    }
  }
  if (fat_part.length() != 0) {
    for (unsigned int i = fat_part.length(); i < block_size; ++i) {
      fat_part.push_back(FILL_CHAR);
    }
    fat_vector.push_back(fat_part);
  }

  // Place the fat blocks
  for (unsigned int i = 0; i < fat_vector.size(); ++i) {
    if (!Vdisk::putBlock(FAT_BLOCK + i, fat_part)) {
      return 0;
    }
  }
  return 1;
}


int FileSys::makeFile(const std::string filename) {
  if (root.size() >= MAX_ROOT_SIZE) return 0;
  if (fat[0] == 0) return 0;
  if (root.find({filename}) != root.end()) return 0;

  root.insert({filename, 0});
  return sync();
}


int FileSys::removeFile(const std::string filename) {
  std::set<RootEntry>::const_iterator it = root.find({filename});
  if (it == root.end()) return 0;
  if (it->startblock != 0) return 0;

  fat[it->startblock] = fat[0];
  root.erase(it);
  return sync();
}


unsigned int FileSys::getFirstBlock(const std::string filename) const {
  std::set<RootEntry>::const_iterator it = root.find({filename});
  if (it == root.end()) return -1;
  return it->startblock;
}


int FileSys::addBlock(const std::string filename, const std::string buffer) {
  std::set<RootEntry>::const_iterator it = root.find({filename});

  if (it == root.end()) return 0;
  if (fat[0] == 0) return -1;

  unsigned int block = fat[0];
  unsigned int currblock = it->startblock;

  fat[0] = fat[block];
  fat[block] = 0;

  if (currblock != 0) {
    while (fat[currblock] != 0) {
      currblock = fat[currblock];
    }
    fat[currblock] = block;
  } else {
    root.erase(it);
    root.insert({filename, block});
  }

  Vdisk::putBlock(block, buffer);
  return sync();
}


int FileSys::deleteBlock(const std::string filename,
                         const unsigned int iBlock) {
  std::set<RootEntry>::const_iterator it = root.find({filename});
  if (it == root.end()) return 0;

  if (it->startblock == iBlock) {
    root.erase(it);
    root.insert({filename, fat[iBlock]});
    fat[iBlock] = fat[0];
    fat[0] = iBlock;
    return 1;
  }

  unsigned int previousBlock = it->startblock;
  while (fat[previousBlock] != iBlock) {
    if (fat[previousBlock] == END) {
      return 0;
    }
    previousBlock = fat[previousBlock];
  }
  fat[previousBlock] = fat[iBlock];
  fat[iBlock] = fat[0];
  fat[0] = iBlock;

  return sync();
}


int FileSys::readBlock(const std::string filename, const unsigned int iBlock,
                       std::string& buffer) const {
  std::set<RootEntry>::const_iterator it = root.find({filename});
  if (it == root.end()) return 0;
  unsigned int block = it->startblock;
  while (block != iBlock) {
    if (block == END) {
      return 0;
    }
    block = fat[block];
  }
  return Vdisk::getBlock(iBlock, buffer);
}


int FileSys::writeBlock(const std::string filename, const unsigned int iBlock,
                        const std::string buffer) const {
  std::set<RootEntry>::const_iterator it = root.find({filename});
  if (it == root.end()) {
    return 0;
  }
  unsigned int block = it->startblock;
  while (block != iBlock) {
    if (block == END) {
      return 0;
    }
    block = fat[block];
  }
  return Vdisk::putBlock(iBlock, buffer);
}


int FileSys::getNextBlock(const std::string filename,
                          const unsigned int iBlock) const {
  std::set<RootEntry>::const_iterator it = root.find({filename});
  if (it == root.end()) return -1;
  unsigned int block = it->startblock;
  while (block != iBlock) {
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
  std::string raw, filename;
  unsigned int address;
  std::stringstream ss;

  while (sroot[0] != FILL_CHAR) {
    filename = sroot.substr(0, FILE_NAME_LENGTH);
    sroot.erase(0, FILE_NAME_LENGTH + 1);
    strip(filename, FILL_CHAR);
    raw = sroot.substr(0, ADDRESS_SPACE);
    sroot.erase(0, ADDRESS_SPACE);
    strip(raw, ' ');
    ss << raw;
    if (ss >> address) {
      root.insert({filename, address});
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
