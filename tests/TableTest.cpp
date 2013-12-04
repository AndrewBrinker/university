/*
 * Copyright 2013 Andrew Brinker
 */

// This is a collection of tests for the Table class.

#ifndef DOCFS_TESTS_TABLE_CPP
#define DOCFS_TESTS_TABLE_CPP

#include <docFS/Table.h>
#include <string>
#include <vector>
#include <iostream>

int tableQuery(Table &table, std::string query) {
    std::string date = "";
    for (unsigned int i = 0; i < query.length(); ++i) {
        if (query[i] == '=') {
            for (unsigned int j = i; j < query.length(); ++j) {
                date += query[j];
            }
            break;
        }
    }
    if (date == "") return 1;
    std::cout << "Searching..." << std::endl;
    table.search(date);
    return 0;
}


int tableTest(std::string& buffer) {
  std::cout << "Testing the Table class" << std::endl;
  Table table("table1", "flat1", "index1");
  std::cout << "Building the Table" << std::endl;
  table.buildTable("data.txt");
  std::cout << "Querying the Table" << std::endl;
  tableQuery(table, "What record has date=50");
  return 1;
}

#endif  // DOCFS_TESTS_TABLE_CPP
