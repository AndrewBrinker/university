/*
 * Copyright 2013 Andrew Brinker
 */

// This is a small collection of tests for the VirtualDisk class. It verifies
// that PutBlock() and GetBlock() work correctly.

#ifndef SRC_TESTS_VDISK_CPP
#define SRC_TESTS_VDISK_CPP

#include <docfs/Vdisk.h>
#include <string>
#include <iostream>


int virtualDiskTest(std::string& buffer) {
  std::cout << "Testing the Vdisk class" << std::endl;

  // Create a new vd1 called test 1 with
  // 16 blocks of 32 bytes
  Vdisk vd1("vd1", 16, 32);

  // Define input blocks
  std::string input1 = "";
  std::string input2 = "";

  // Define output blocks
  std::string output1 = "";
  std::string output2 = "";

  // Append characters to blocks 1 and 2
  for (unsigned int i = 1; i <= 32; ++i) {
      input1 = input1 + "1";
  }
  for (unsigned int i = 1; i <= 32; ++i) {
      input2 = input2 + "2";
  }

  // Put and get the block, and verify they are identical
  vd1.putBlock(4, input1);
  vd1.getBlock(4, output1);
  if (input1 != output1) {
    buffer = "putBlock and getBlock are not equivalent.";
    return 0;
  }

  // Put and get the block, and verify they are identical
  vd1.putBlock(8, input2);
  vd1.getBlock(8, output2);
  if (input2 != output2) {
    buffer = "putBlock and getBlock are not equivalent.";
    return 0;
  }

  return 1;
}

#endif  // SRC_TESTS_VDISK_CPP
