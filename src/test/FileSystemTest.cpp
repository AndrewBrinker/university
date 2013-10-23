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

// This is a collection of tests for the FileSystem class.

#ifndef DOCFS_SRC_TEST_FILESYSTEM_CPP_
#define DOCFS_SRC_TEST_FILESYSTEM_CPP_

#include <string>
#include <vector>
#include <iostream>
#include "../FileSystem.h"

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
  std::cout << "Testing the FileSystem class" << std::endl;

  VirtualDisk disk1("disk1",256,128);
  FileSystem fsys("disk1");
  fsys.newFile("file1");
  fsys.newFile("file2");

  std::string bfile;

  for (int i = 1; i <= 1024; ++i) {
    bfile += "1";
  }

  std::vector<std::string> blocks = block(bfile, 128);
  int blocknumber = 0;

  for (int i = 0; i <= blocks.size(); ++i) {
    blocknumber = fsys.addBlock("file1", blocks[i]);
  }

  fsys.deleteBlock("file1", fsys.getFirstBlock("file1"));

  for (int i = 1; i <= 2048; ++i) {
    bfile += "2";
  }

  for (int i = 0; i <= blocks.size(); ++i) {
    blocknumber = fsys.addBlock("file2", blocks[i]);
  }

  fsys.deleteBlock("file2", blocknumber);

  return 1;
}

#endif  // DOCFS_SRC_TEST_FILESYSTEM_CPP_
