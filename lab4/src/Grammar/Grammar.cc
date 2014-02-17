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
   * 2) if X->e, add e to FIRST(X)
   * 3) if X is a nonterminal with some production, check each value on the
   *    right of the production for e. If it has e, move to the next one, if it
   *    doesn't, add FIRST(Y_i). If you get to the end, add e.
   * 4) Repeat until no change.
   */
  for (int i = 0; i < _productions.size(); ++i) {
    // For each production
    const char left_side         = _productions[i][0];
    const std::string right_side = _productions[i].substr(3);
    for (int j = 0; i < right_side.size(); ++j) {
      char current = right_side[j];
      if (isTerminal(current)) {
        // Add current to FIRST(current)
      }
    }
  }
  return false;
}


bool Grammar::getFollow() {
  return false;
}




