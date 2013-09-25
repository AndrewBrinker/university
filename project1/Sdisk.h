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

#ifndef SDISK_H
#define SDISK_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#include <string>

class Sdisk {
public:
    explicit Sdisk(std::string new_disk_name);
    Sdisk(std::string new_disk_name, unsigned int new_number_of_blocks, unsigned int new_block_size);
    unsigned int getblock(unsigned int block_number, std::string& buffer);
    unsigned int putblock(unsigned int block_number, std::string buffer);
private:
    inline unsigned int fileexists(std::string filename);
    inline void createfile(std::string filename);
    std::string disk_name;         // Filename of software disk
    unsigned int number_of_blocks; // Number of blocks on disk
    unsigned int block_size;       // Block size in bytes

    DISALLOW_COPY_AND_ASSIGN(Sdisk);
};

#endif
