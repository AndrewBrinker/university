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
  bool changed;
  do {
    changed = false;
    for (auto production : _productions) {
      size_t i = 1;
      char lhs = production[0];
      std::string rhs = production.substr(3);
      while (i < rhs.length()) {
        std::set<char> first = _first[rhs[i]];
        if (hasEpsilon(first)) {
          first.erase(first.find(EPSILON_CHAR));
          addToFirst(first, lhs, changed);
          ++i;
        } else {
          addToFirst(first, lhs, changed);
        }
        if (i >= rhs.length()) {
          auto result = _first[lhs].insert(EPSILON_CHAR);
          if (result.second) changed = true;
        }
        ++i;
      }
    }
  } while (changed);
}


bool Grammar::hasEpsilon(std::set<char> first) {
  return first.find(EPSILON_CHAR) != first.end();
}


void Grammar::addToFirst(std::set<char> first,
                         char nonterminal,
                         bool &changed) {
  for (auto symbol : first) {
    auto result = _first[nonterminal].insert(symbol);
    if (result.second) changed = true;
  }
}


bool Grammar::findFollow() {
  return false;
}


mapset Grammar::first() const {
  return _first;
}


mapset Grammar::follow() const {
  return _follow;
}

