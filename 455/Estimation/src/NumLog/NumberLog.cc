/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 *
 * Copyright 2014 Andrew Brinker
 */

#include "./NumberLog.h"
#include <sys/stat.h>
#include <list>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>


NumberLog::NumberLog(std::string name) : file_name(name) {
  input();
}


void NumberLog::input() {
  while (true) {
    if (fileExists(file_name)) break;
    std::cout << "This file does not exist. Please choose another file name."
              << std::endl
              << "Read from file: ";
    std::cin >> file_name;
  }
  std::ifstream file_stream(file_name);
  for (std::string line; std::getline(file_stream, line);) {
    data.push_back(atof(line.c_str()));
  }
  file_stream.close();
}


bool NumberLog::fileExists(std::string file_name) {
  struct stat buffer;
  return stat(file_name.c_str(), &buffer) == 0;
}

