/*
 * Copyright 2013 Andrew Brinker
 */

// This is the central testing program.

#include <docFs/Vdisk.h>
#include <docFS/FileSys.h>
#include <docFS/Table.h>
#include <iostream>
#include <string>
#include <vector>

int virtualDiskTest(std::string& buffer);
int fileSystemTest(std::string& buffer);
int tableTest(std::string& buffer);

/*=============================================================================
 * Test Dispatcher
 ============================================================================*/

int main() {
  std::string buffer = "";
  if (!virtualDiskTest(buffer)) {
    std::cout << "VirtualDisk testing failed with the error:\n    "
              << buffer << std::endl;
    return 0;
  }
  if (!fileSystemTest(buffer)) {
    std::cout << "FileSystem testing failed with the error:\n    "
              << buffer << std::endl;
    return 0;
  }
  if (!tableTest(buffer)) {
    std::cout << "Table testing failed with the error:\n    "
              << buffer << std::endl;
    return 0;
  }
  std::cout << "All tests passed." << std::endl;
  return 1;
}


/*=============================================================================
 * Vdisk Testing
 ============================================================================*/

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
      input1 = input1 + "1"
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


int fileSystemTest(std::string& buffer) {
  std::cout << "Testing the FileSys class" << std::endl;

  Vdisk disk1("disk1", 100, 500);
  FileSys fsys("disk1");
  fsys.makeFile("file1###########");
  fsys.makeFile("file2###########");

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


/*=============================================================================
 * Table Testing
 ============================================================================*/

void strip(std::string& str, const char c = FILL_CHAR) {
  while (str.front() == c) str.erase(0, 1);
  while (str.back() == c) str.erase(str.length() - 1);
}

int tableQuery(Table &table, std::string query) {
    std::string date = "";
    for (unsigned int i = 0; i < query.length(); ++i) {
        if (query[i] == '=') {
            for (unsigned int j = i + 1; j < query.length(); ++j) {
                date += query[j];
            }
            break;
        }
    }
    if (date == "") return 1;
    strip(date, ' ');
    std::cout << date << std::endl;
    table.search(date);
    return 0;
}


int tableTest(std::string& buffer) {
  if (false) {
    buffer = "";
  }
  std::cout << "Testing the Table class" << std::endl;
  Table table("table1", "flat1", "index1");
  table.buildTable("data.txt");

  std::string input;
  do {
    getline(std::cin, input, '\n');
    tableQuery(table, input);
  } while (std::cin.good());
  return 1;
}
