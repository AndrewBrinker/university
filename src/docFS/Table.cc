/*
 * Copyright 2013 Andrew Brinker
 */

#include <cstring>
#include <cstdio>
#include <string>
#include <fstream>
#include "./Table.h"

#define RECORDS_PER_BLOCK (DEFAULT_DEFAULT_BLOCK_SIZE / MAX_RECORD_LENGTH)

Table::Table(std::string new_diskname,
             std::string new_flat_file,
             std::string new_index_file)
           : FileSys(new_diskname),
             flat_file(new_flat_file),
             index_file(new_index_file) {
    makeFile(new_flat_file);
    makeFile(new_index_file);
}


int Table::buildTable(std::string inputFile) {
  std::ifstream iFile(inputFile);

  std::string record;
  std::string flat_block;
  std::string index_block;
  std::string date;
  char index[DATE_LENGTH + 1];
  unsigned int flatFileBlock = 0;
  unsigned int offset;

  record.reserve(MAX_RECORD_LENGTH);
  flat_block.reserve(DEFAULT_BLOCK_SIZE);
  index_block.reserve(DEFAULT_BLOCK_SIZE);
  date.reserve(DATE_LENGTH);

  while (true) {
    std::getline(iFile, record);

    if (record.length() == 0) {
      break;
    }

    while (record.length() < MAX_RECORD_LENGTH) {
      record.push_back(FILL_CHAR);
    }

    flat_block += record;

    if (flat_block.length() + MAX_RECORD_LENGTH <= DEFAULT_BLOCK_SIZE) {
      continue;
    }

    while (flat_block.length() < DEFAULT_BLOCK_SIZE) {
      flat_block.push_back(FILL_CHAR);
    }

    if (addBlock(flat_file, flat_block) <= 0) {
      return 0;
    }

    if (flatFileBlock == 0) {
      flatFileBlock = getFirstBlock(flat_file);
    } else {
      flatFileBlock = getNextBlock(flat_file, flatFileBlock);
    }

    sprintf(index, "%*d", DATE_LENGTH, flatFileBlock);

    for (offset = 0;
         offset < flat_block.length();
         offset += MAX_RECORD_LENGTH) {
      date = flat_block.substr(offset, DATE_LENGTH);
      if (date[0] == FILL_CHAR) {
        continue;
      }
      index_block += date;
      index_block += index;

      if (index_block.length() + INDEX_RECORD_LENGTH > DEFAULT_BLOCK_SIZE) {
        addBlock(index_file, index_block);
        index_block.erase();
      }
    }
    flat_block.erase();
  }
  iFile.close();
  return 1;
}


int Table::search(std::string value) {
  int block = indexSearch(value);
  if (block == 0) return block;
  std::string block_contents(DEFAULT_BLOCK_SIZE, FILL_CHAR);
  readBlock(flat_file, block, block_contents);

  for (size_t i = 0; i < block_contents.length(); i += MAX_RECORD_LENGTH) {
    if (block_contents.substr(i, value.length()) == value) {
      size_t j = i;
      while (block_contents[j] != FILL_CHAR && j - i < MAX_RECORD_LENGTH) {
        putchar(block_contents[j++]);
      }
      putchar('\n');
      return 1;
    }
  }
  return 0;
}


int Table::indexSearch(std::string value) {
  int index_block_number = getFirstBlock(index_file);
  int flat_block_number;
  int i_value;
  int date;
  std::string index_record;
  std::string index_block_contents(DEFAULT_BLOCK_SIZE, FILL_CHAR);
  sscanf(value.c_str(), "%d", &i_value);

  do {
    readBlock(index_file, index_block_number, index_block_contents);

    for (size_t i = 0;
         i < index_block_contents.length();
         i += INDEX_RECORD_LENGTH) {
      index_record = index_block_contents.substr(i, INDEX_RECORD_LENGTH);
      sscanf(index_record.c_str(), "%d %d", &date, &flat_block_number);
      if (i_value == date) {
        return flat_block_number;
      }
    }
  } while ((index_block_number = getNextBlock(index_file, index_block_number)));
  return 0;
}
