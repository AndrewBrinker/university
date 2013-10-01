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

// This is the interface for the FileSystem class. This is the actual file
// system, and it does all of the heavy lifting, using VirtualDisk as the
// back end. All the warnings that apply to VirtualDisk apply to FileSystem.

#ifndef DOCFS_SRC_FILESYSTEM_H_
#define DOCFS_SRC_FILESYSTEM_H_

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#include <string>
#include <vector>
#include "./VirtualDisk.h"


class FileSystem : public VirtualDisk {
 public:
  explicit FileSystem(std::string name);
  unsigned int fileClose();
  unsigned int fileSync();
  unsigned int newFile(std::string file);
  unsigned int removeFile(std::string file);
  unsigned int getFirstBlock(std::string file);
  unsigned int nextBlock(std::string file, unsigned int block_number);
  unsigned int addBlock(std::string file, std::string block);
  unsigned int deleteBlock(std::string file, unsigned int block_number);
  unsigned int getBlock(std::string file,
                        unsigned int block_number,
                        std::string& buffer);
  unsigned int putBlock(std::string file,
                        unsigned int block_number,
                        std::string buffer);

 private:
  unsigned int root_size;                 // maximum number of entries in ROOT
  unsigned int fat_size;                  // number of blocks occupied by FAT
  std::vector<std::string> file_name;     // filenames in ROOT
  std::vector<unsigned int> first_block;  // firstblocks in ROOT
  std::vector<unsigned int> fat;          // FAT

  DISALLOW_COPY_AND_ASSIGN(FileSystem);
};


#endif  // DOCFS_SRC_FILESYSTEM_H_
