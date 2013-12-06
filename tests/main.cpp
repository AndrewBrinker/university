/*
 * Copyright 2013 Andrew Brinker
 */

// This is the central testing program.

#include <docFs/Vdisk.h>
#include <docFS/FileSys.h>
#include <docFS/Table.h>
#include <docFS/FastTable.h>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>

int vdiskTest();
int fileSysTest();
int tableTest();
int fastTableTest();

/*=============================================================================
 * Test Dispatcher
 ============================================================================*/

int main() {
  std::cout << "\nStarting testing:\n" << std::endl;

  std::cout << "Testing the Vdisk class..." << std::endl;
  if (!vdiskTest()) {
    std::cout << "  - Vdisk testing failed." << std::endl;
    return 0;
  }
  std::cout << "  + Vdisk testing succeeded!" << std::endl;
  std::cout << "Testing the FileSys class..." << std::endl;
  if (!fileSysTest()) {
    std::cout << "  - FileSys testing failed." << std::endl;
    return 0;
  }
  std::cout << "  + FileSys testing succeeded!" << std::endl;
  std::cout << "Testing the Table class..." << std::endl;
  if (!tableTest()) {
    std::cout << "  - Table testing failed." << std::endl;
    return 0;
  }
  std::cout << "  + Table testing succeeded!" << std::endl;
  std::cout << "Testing the FastTable class..." << std::endl;
  if (!fastTableTest()) {
    std::cout << "  - FastTable testing failed." << std::endl;
    return 0;
  }
  std::cout << "  + FastTable testing succeeded!" << std::endl;
  std::cout << "\nAll tests passed!\n" << std::endl;
  return 1;
}


/*=============================================================================
 * Vdisk Testing
 ============================================================================*/

int vdiskTest() {
  // Create a new vd1 called test 1 with
  // 16 blocks of 32 bytes
  Vdisk vd1("vdisk1", 16, 32);

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
    return 0;
  }

  // Put and get the block, and verify they are identical
  vd1.putBlock(8, input2);
  vd1.getBlock(8, output2);
  if (input2 != output2) {
    return 0;
  }

  return 1;
}


/*=============================================================================
 * FileSys Testing
 ============================================================================*/

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


int fileSysTest() {
  Vdisk disk1("filesys1", 100, 500);
  FileSys fsys("filesys1");
  fsys.makeFile("file1");
  fsys.makeFile("file2");

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

  return 1;
}


/*=============================================================================
 * Table Testing
 ============================================================================*/

void prep_query(std::string& str) {
  while (!isdigit(str.front()) && str.front() != '-') str.erase(0, 1);
  while (!isdigit(str.back())) str.erase(str.length() - 1);
}


int tableTest() {
  Table table("table1", "flat1", "index1");
  table.buildTable("data.txt");
  std::cout << "For testing you're limited to 4 searches" << std::endl;
  for (int i = 0; i < 4; ++i) {
    std::cout << "\nWhat date do you want to know about?\n    > ";
    std::string input;
    getline(std::cin, input);
    prep_query(input);
    table.search(input);
  }
  return 1;
}


/*=============================================================================
 * FastTable Testing
 ============================================================================*/

int fastTableTest() {
  FastTable fast_table("fast_table1", "flat1", "index1");
  return 1;
}

