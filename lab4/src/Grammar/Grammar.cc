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


/** Print the contents of a set
 *
 * Iterates through a set and prints all the elements on a single line.
 */
template <class T>
void print_set(std::set<T> input) {
  for (auto it = input.begin(); it != input.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
}


/** Do nothing
 *
 * Constructors can't return values or throw error, and are therefore a poor
 * place to do work. The constructor does absolutely nothing. Use load().
 */
Grammar::Grammar() {}


/**
 * Load the contents of the given grammar into the class.
 * @param  file_name -> name of the file being loaded
 * @return           -> exit code
 */
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


/**
 * Finds the first and follow sets for the grammar
 * @return exit code
 */
int Grammar::parse() {
  bool failed = findFirst();
  if (failed) return 1;
  failed = findFollow();
  return failed;
}


/**
 * Finds the first set for the grammar
 * @return exit code
 */
bool Grammar::findFirst() {
  firstRuleOne();
  firstRuleTwo();
  firstRuleThree();
  return false;
}


/**
 * Applies the first rule for finding the follow sets
 */
void Grammar::firstRuleOne() {
  for (auto it = _terminals.begin(); it != _terminals.end(); ++it) {
    _first[*it].insert(*it);
  }
}


/**
 * Applies the second rule for finding the follow sets
 */
void Grammar::firstRuleTwo() {
  for (auto it = _productions.begin(); it != _productions.end(); ++it) {
    if (it->substr(3) == EPSILON_STR) {
      _first[it->substr(0, 3)[0]].insert(EPSILON_CHAR);
    }
  }
}


/**
 * Applies the third rule for finding the follow sets
 */
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
          addToFirst(lhs, first, changed);
          ++i;
        } else {
          addToFirst(lhs, first, changed);
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


/**
 * Checks whether epsilon is present in the given set.
 * @param  first -> The set being checked for epsilon
 * @return       -> The result of the test
 */
bool Grammar::hasEpsilon(std::set<char> first) {
  return first.find(EPSILON_CHAR) != first.end();
}


/**
 * Add the given first set to the first of the given nonterminal
 * @param nonterminal -> The symbol whose first is being added to
 * @param first       -> The symbols being added
 * @param changed     -> A flag to see if anything actually changed
 */
void Grammar::addToFirst(char nonterminal,
                         std::set<char> first,
                         bool &changed) {
  for (auto symbol : first) {
    auto result = _first[nonterminal].insert(symbol);
    if (result.second) changed = true;
  }
}


/**
 * Does nothing right now
 * @return exit code
 */
bool Grammar::findFollow() {
  return false;
}


/**
 * Returns the first sets for the grammar
 * @return the first sets
 */
mapset Grammar::first() const {
  return _first;
}


/**
 * Returns the follow sets for the grammar
 * @return the follow sets
 */
mapset Grammar::follow() const {
  return _follow;
}

