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

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "./Table.h"

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
             FileSystem(new_diskname),
             flat_file(new_flat_file),
             index_file(new_index_file) {
    FileSystem::newFile(new_flat_file);
    FileSystem::newFile(new_index_file);
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
            record_string.push_back(records[i]);
        }
        record_string = FileSystem::block(record_string)[0];

        FileSystem::addBlock(flat_file);
        unsigned int current_block = FileSystem::getFirstBlock(flat_file);
        while (FileSystem::getNextBlock(flat_file, current_block) != 0) {
            current_block = FileSystem::getNextBlock(flat_file, current_block);
        }
        // current block now equals the block number the stuff was written out to.
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
        indices = FileSystem::block(indices);
        for (unsigned int i = 0; i < indices.size(); ++i) {
            FileSystem::addBlock(index_file, indices[i]);
        }
    }
    return 0;
}


unsigned int Table::indexSearch(std::string value) {
    std::string blah = value;
    // 1) Read in the blocks for the index to a string.
    // 2) Starting at the first 5 bytes, jump from date to date until a match
    //    is found.
    // 3) When found, load the block containing the record.
    // 4) Search the block for the specific record you need.
    return 0;
}
