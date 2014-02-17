/**
 * Copyright 2014 Andrew Brinker
 */

#include "./Grammar.h"
#include <set>
#include <string>
#include <fstream>


Grammar::Grammar() {}


int Grammar::load(std::string file_name) {
  _name = file_name;
  std::ifstream input_file(_name);
  for (std::string line; getline(input_file, line);) {
    if (line == "$") break;
    _terminals.push_back(line);
  }
  for (std::string line; getline(input_file, line);) {
    if (line == "$") break;
    _productions.push_back(line);
  }
  return 0;
}


int Grammar::parse() {
  bool failed = getFirst();
  if (failed) return 1;
  failed = getFollow();
  return failed;
}


std::vector<std::set<std::string>> Grammar::first() const {
  return _first;
}


std::vector<std::set<std::string>> Grammar::follow() const {
  return _follow;
}


bool Grammar::getFirst() {
  /*
   * 1) if X is a terminal, FIRST(X) = X
   */
  return false;
}


bool Grammar::getFollow() {
  return false;
}




