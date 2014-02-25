/**
 * Copyright 2014 Andrew Brinker
 */

#include "./Grammar.h"
#include <set>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#define EOI_STR "$"
#define EOI_CHAR '$'
#define EPSILON_STR "e"
#define EPSILON_CHAR 'e'

template <class T>
void print_set(std::set<T> input) {
  for (auto it = input.begin(); it != input.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}


Grammar::Grammar() {}


int Grammar::load(std::string file_name) {
  std::ifstream input_file(file_name);
  // The terminals section
  for (std::string line; getline(input_file, line);) {
    if (line == EOI_STR) break;
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
  firstRuleOne();
  firstRuleTwo();
  firstRuleThree();
  return false;
}


void Grammar::firstRuleOne() {
  for (auto it = _terminals.begin(); it != _terminals.end(); ++it) {
    _first[*it].insert(*it);
  }
}


void Grammar::firstRuleTwo() {
  for (auto it = _productions.begin(); it != _productions.end(); ++it) {
    if (it->substr(3) == EPSILON_STR) {
      _first[it->substr(0, 3)[0]].insert(EPSILON_CHAR);
    }
  }
}


void Grammar::firstRuleThree() {
  // Variable to track whether a change was made in the last sweep
  // Keep checking until no change occurs
  bool changed;
  do {
    changed = false;
    // Iterate through all the productions
    for (auto it = _productions.begin(); it != _productions.end(); ++it) {
      // Get the right hand side of the current production
      std::string rhs = it->substr(3);
      // Iterate through the right hand side of the current production
      size_t i = 1;
      while (i < rhs.length()) {
        // Get the FIRST of the rhs of the current production
        std::set<char> current_first = _first[rhs[i]];
        print_set(current_first);
        // If epsilon is present in that FIRST, then move on to the next char
        if (current_first.find(EPSILON_CHAR) != current_first.end()) {
          // Add all of current first except epsilon to _first[lhs[0]]
          std::set<char> temp_first = current_first;
          temp_first.erase(temp_first.find(EPSILON_CHAR));
          std::string lhs = it->substr(0, 3);
          for (auto it2 = temp_first.begin();
                    it2 != temp_first.end();
                    ++it2) {
            auto result = _first[lhs[0]].insert(*it2);
            if (result.second) changed = true;
          }
          ++i;
        } else {
          // Otherwise, add the FIRST of the current char to the FIRST of the
          // lhs
          std::string lhs = it->substr(0, 3);
          for (auto it2 = current_first.begin();
               it2 != current_first.end();
               ++it2) {
            auto result = _first[lhs[0]].insert(*it2);
            if (result.second) changed = true;
          }
        }
        // If nothing has been added yet (that is, everything is epsilons) add
        // epsilon.
        if (i >= rhs.length()) {
          std::string lhs = it->substr(0, 3);
          auto result = _first[lhs[0]].insert(EPSILON_CHAR);
          if (result.second) changed = true;
        }
      }
    }
  } while (changed);
}


bool Grammar::findFollow() {
  return false;
}

