/*
 * Copyright 2013 Andrew Brinker
 */

// This is a collection of tests for the FileSystem class.

#ifndef DOCFS_TESTS_FILESYS_CPP
#define DOCFS_TESTS_FILESYS_CPP

#include <docFS/FileSys.h>
#include <string>
#include <vector>
#include <iostream>

std::vector<std::string> block(std::string blocks, unsigned int block_size) {
  std::string partial;
  std::vector<std::string> result;
  for (unsigned int i = 0; i < blocks.length(); ++i) {
    char c = blocks[i];
    if (partial.length() < block_size) {
      partial.push_back(c);
    } else {
      result.push_back(partial);
      partial.clear();
    }
  }
  if (partial.length() != 0) {
    // Make sure to add the last block and pad it if it's not the right size.
    for (unsigned int i = partial.length(); i < block_size; ++i) {
      partial.push_back('#');
    }
    result.push_back(partial);
    partial.clear();
  }
  return result;
}


int fileSystemTest(std::string& buffer) {
  std::cout << "Testing the FileSys class" << std::endl;

  Vdisk disk1("disk1", 100, 500);
  FileSys fsys("disk1");
  fsys.newFile("file1###########");
  fsys.newFile("file2###########");

  std::string bfile;

  for (int i = 1; i <= 1500; ++i) {
    bfile += "1";
  }

  std::vector<std::string> blocks = block(bfile, 500);
  int blocknumber = 0;

  for (unsigned int i = 0; i < blocks.size(); ++i) {
    blocknumber = fsys.addBlock("file1", blocks[i]);
  }

  fsys.deleteBlock("file1", fsys.getFirstBlock("file1"));

  for (unsigned int i = 1; i <= 2000; ++i) {
    bfile += "2";
  }

  for (unsigned int i = 0; i < blocks.size(); ++i) {
    blocknumber = fsys.addBlock("file2", blocks[i]);
  }

  fsys.deleteBlock("file2", blocknumber);

  if (false) {
    // Will write tests later.
    buffer = "";
  }

  return 1;
}

#endif  // DOCFS_TESTS_FILESYS_CPP
