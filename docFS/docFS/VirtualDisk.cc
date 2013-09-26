// The MIT License (MIT)
// 
// Copyright (c) 2013 Andrew Brinker
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// This is the implementation of the class VirtualDisk defined in VirtualDisk.h
// It defines all non-inline functions of the class.

#include "VirtualDisk.h"
#include <string>
#include <fstream>
#include <iostream>


// Initializes a disk with nothing but a name.
// Assumes the disk already exists. It does nothing to create new files,
// it only attaches the instance to a disk that already exists.
// It is the programmer's responsibility to make sure the name matches
// the file you intend to access.
VirtualDisk::VirtualDisk(std::string current_name) {
  name = current_name;
  std::string arch_file = name + ".spc";
  std::string data_file = name + ".dat";
    
}


// Initializes a disk with a name, and a number of blocks of a certain size.
// This constructor creates a new disk with the number of blocks and block size
// given by the parameters.
VirtualDisk::VirtualDisk(std::string new_name,
                         unsigned int new_number_of_blocks,
                         unsigned int new_block_size) {
  name = new_name;
  number_of_blocks = new_number_of_blocks;
  block_size = new_block_size;
  std::string arch_file = name + ".spc";
  std::string data_file = name + ".dat";
    
}


// Retrieves a block and stores it into the buffer.
// It returns 1 if successful and 0 otherwise.
unsigned int VirtualDisk::GetBlock(unsigned int block_number,
                                   std::string& buffer) {
  return 1;
}


// Writes the buffer to the block.
// Returns 1 if successful and 0 otherwise.
unsigned int VirtualDisk::PutBlock(unsigned int block_number,
                                   std::string buffer) {
  return 1;
}