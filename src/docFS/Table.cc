/*
 * Copyright 2013 Andrew Brinker
 */

#include <string>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "./Table.h"

#define BLOCK_SIZE         500
#define FILL_CHAR          '#'
#define MAX_RECORD_LENGTH  120
#define DATE_LENGTH        5
#define END_LENGTH         5
#define TYPE_LENGTH        8
#define PLACE_LENGTH       15
#define REFERENCE_LENGTH   7
#define DESCRIPTION_LENGTH (MAX_RECORD_LENGTH - DATE_LENGTH \
                           - END_LENGTH - TYPE_LENGTH \
                           - PLACE_LENGTH - REFERENCE_LENGTH)
#define INDEX_LENGTH       DATE_LENGTH + END_LENGTH


Table::Table(std::string new_diskname,
             std::string new_flat_file,
             std::string new_index_file):
             FileSys(new_diskname),
             flat_file(new_flat_file),
             index_file(new_index_file) {
    FileSys::newFile(new_flat_file);
    FileSys::newFile(new_index_file);
}


unsigned int Table::buildTable(std::string input_file) {
    std::ifstream read_input(input_file);
    // 1) Read in a number of records that can be contained in a block.
    // 2) Pad that block to the full block size.
    // 3) Add a block to flat_file containing those records
    // 4) Update index_file containing the dates just saved, and the block they
    //    were saved to.
    // 5) Repeat until all records have been saved.
    while (read_input.good()) {
        std::string record = "";
        unsigned int count = 0;
        std::vector<std::string> records;
        std::vector<std::string> dates;

        do {
            record.clear();
            getline(read_input, record);
            dates.push_back(record.substr(0, 5));
            records.push_back(record);
            ++count;
        } while (count < (BLOCK_SIZE / MAX_RECORD_LENGTH));
        std::string record_string = "";
        for (unsigned int i = 0; i < records.size(); ++i) {
            record_string.append(records[i]);
        }
        record_string = FileSys::block(record_string)[0];

        unsigned int current_block = FileSys::getFirstBlock(flat_file);
        while (FileSys::getNextBlock(flat_file, current_block) != 0) {
            current_block = FileSys::getNextBlock(flat_file, current_block);
        }
        // current block now equals the block number the stuff was written to.
        // now I need to save to the index_file.
        // I need to turn current_block into a string of 5 characters.
        std::ostringstream outs;
        outs << current_block;
        std::string block_string = outs.str();
        for (unsigned int i = block_string.length(); i < 5; ++i) {
            block_string.push_back(' ');
        }
        std::vector<std::string> indices;
        for (unsigned int i = 0; i < dates.size(); ++i) {
            indices.push_back(dates[i] + block_string);
        }
        std::string indices_string;
        for (unsigned int i = 0; i < indices.size(); ++i) {
            indices_string.append(indices[i]);
        }
        indices = FileSys::block(indices_string);
        for (unsigned int i = 0; i < indices.size(); ++i) {
            FileSys::addBlock(index_file, indices[i]);
        }
    }
    return 0;
}


unsigned int Table::search(std::string value) {
    unsigned int block = indexSearch(value);
    if (block == 0) {
        return 0;
    }
    std::string block_contents(BLOCK_SIZE, FILL_CHAR);
    FileSys::readBlock(flat_file, block, block_contents);
    for (std::string::const_iterator it = block_contents.cbegin();
         it != block_contents.end();
         ++it) {
        if (*it == '\n' || it == block_contents.cbegin()) {
            if (strncmp(&*(++it), value.c_str(), DATE_LENGTH) == 0) {
                while (*it != '\n') {
                    std::putchar(*it);
                }
                std::putchar('\n');
                return 1;
            }
        }
    }
    return 0;
}


unsigned int Table::indexSearch(std::string value) {
    int current_block = FileSys::getFirstBlock(index_file);
    std::string block_contents(BLOCK_SIZE, FILL_CHAR);
    do {
        FileSys::readBlock(index_file, current_block, block_contents);
        for (std::string::const_iterator it = block_contents.cbegin();
             it != block_contents.cend();
             it += INDEX_LENGTH) {
            if (strncmp(&*it, value.c_str(), DATE_LENGTH) == 0) {
                return current_block;
            }
        }
    } while (current_block == FileSys::getNextBlock(index_file, current_block));
    return 0;
}
