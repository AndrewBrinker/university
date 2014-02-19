/**
 * Copyright 2014 Andrew Brinker
 */

#include "./Grammar.h"
#include <set>
#include <map>
#include <string>
#include <fstream>


Grammar::Grammar() {}


int Grammar::load(std::string file_name) {
  std::ifstream input_file(file_name);
  // The terminals section
  for (std::string line; getline(input_file, line);) {
    if (line == "$") break;
    _terminals.insert(line[0]);
  }
  // The productions section
  for (std::string line; getline(input_file, line);) {
    if (line == "$") break;
    _non_terminals.insert(line[0]);
    _productions.insert(line);
  }
  return 0;
}


int Grammar::parse() {
  bool failed = findFirst();
  if (failed) return 1;
  failed = findFollow();
  return failed;
}


bool Grammar::findFirst() {
  /*
   * 1) if X is a terminal, FIRST(X) = X
   * 2) if X->e, add e to FIRST(X)
   * 3) if X is a nonterminal with some production, check each value on the
   *    right of the production for e. If it has e, move to the next one, if it
   *    doesn't, add FIRST(Y_i). If you get to the end, add e.
   * 4) Repeat until no change.
   */
  for (auto it = _terminals.begin(); it != _terminals.end(); ++it) {
    // Add '*it' to FIRST(*it)
  }
  for (auto it = _productions.begin(); it != _productions.end(); ++it) {
    if (it->substr(3) == "e") {
      // Add 'e' to FIRST(lhs)
    }
  }
  bool changed = false;
  do {
    for (auto p_it = _productions.begin(); p_it != _productions.end(); ++p_it) {
      std::string rhs = it->substr(3);
      for (auto s_it = rhs.begin(); s_it != rhs.end(); ++s_it) {
        std::set<char> curr_first = _first[*it];
        if (curr_first.find('e') != curr_first.end()) {
          ++s_it;
        } else {
          // Add FIRST(*it) to FIRST(lhs)
          changed = true;
        }
        if (s_it == rhs.end()) {
          // Add 'e' to FIRST(lhs)
          changed = true;
        }
      }
    }
  } while(changed);
  return false;
}


bool Grammar::findFollow() {
  return false;
}


std::map<char, std::set<char>> Grammar::first() const {
  return _first;
}


std::map<char, std::set<char>> Grammar::follow() const {
  return _follow;
}

