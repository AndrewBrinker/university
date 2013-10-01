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
#include <string>
#include <fstream>
#include <iostream>
#include <vector>


FileSystem::FileSystem(std::string new_name) : VirtualDisk(new_name) {
  std::cout << std::endl;
}


unsigned int FileSystem::fileClose() {
  return 1;
}


unsigned int FileSystem::fileSync() {
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
