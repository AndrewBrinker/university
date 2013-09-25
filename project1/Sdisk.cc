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

#include "Sdisk.h"
#include <string>
#include <fstream>
#include <iostream>


// Initializes a disk with nothing but a name.
// Assumes the disk already exists. It does nothing to create new files,
// it only attaches the instance to a disk that already exists.
// It is the programmer's responsibility to make sure the name matches
// the file you intend to access.
Sdisk::Sdisk(std::string new_disk_name) {
    disk_name = new_disk_name;
    arch_file = disk_name + ".spc";
    data_file = disk_name + ".dat";
    
}


// Initializes a disk with a name, and a number of blocks of a certain size.
// This constructor creates a new disk with the number of blocks and block size
// given by the parameters.
Sdisk::Sdisk(std::string new_disk_name,
             unsigned int new_number_of_blocks,
             unsigned int new_block_size) {
    disk_name = new_disk_name;
    number_of_blocks = new_number_of_blocks;
    block_size = new_block_size;
    arch_file = disk_name + ".spc";
    data_file = disk_name + ".dat";
    
}


// Retrieves a block and stores it into the buffer.
// It returns 1 if successful and 0 otherwise.
unsigned int Sdisk::getblock(unsigned int block_number, std::string& buffer) {
    return 1;
}


// Writes the buffer to the block.
// Returns 1 if successful and 0 otherwise.
unsigned int Sdisk::putblock(unsigned int block_number, std::string buffer) {
    return 1;
}

// Returns 1 is the file exists, 0 otherwise.
inline unsigned int Sdisk::fileexists(std::string filename) {
    std::ifstream file(filename.c_str());
    return file.good();
}

// Creates the given file if it doesn't already exist.
inline void Sdisk::createfile(std::string filename) {
    if(!Sdisk::fileexists(filename)) {
        std::ofstream file(filename.c_str());
        file.close();
    }
}
