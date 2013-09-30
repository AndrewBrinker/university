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

// This is a small collection of tests for the VirtualDisk class. It verifies
// that PutBlock() and GetBlock() work correctly.

#include <string>
#include <iostream>
#include <cassert>
#include "../VirtualDisk.h"


int main(int argc, char** argv) {
  // Create a new disk1 called test 1 with
  // 16 blocks of 32 bytes
  VirtualDisk disk1("test1", 16, 32);

  // Define input blocks
  std::string input1 = "";
  std::string input2 = "";

  // Define output blocks
  std::string output1 = "";
  std::string output2 = "";    

  // Append characters to blocks 1 and 2
  for(unsigned int i = 1; i <= 32; ++i) {
      input1 = input1 + "1";
  }
  for(unsigned int i = 1; i <= 32; ++i) {
      input2 = input2 + "2";
  }

  // Put and get the block, and verify they are identical
  disk1.putBlock(4, input1);
  disk1.getBlock(4, output1);
  assert(input1 == output1);

  // Put and get the block, and verify they are identical
  disk1.putBlock(8, input2);
  disk1.getBlock(8, output2);
  assert(input2 == output2);

  std::cout << "All tests passed." << std::endl;
}