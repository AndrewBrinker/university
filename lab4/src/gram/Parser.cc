/**
 * Copyright 2014 Andrew Brinker
 */

#include "./Parser.h"
#include <cstdio>
#include <cctype>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <list>
#include <vector>
#include <iostream>
#include "./Preprocessor.h"
#include "./Grammar.h"

#define DELIM     "$"
#define EPSILON   "e"

/**
 * Load the contents of the given Parser into the class
 * @param  file_name -> name of the file being loaded
 */
Parser::Parser(const std::string file_name) {
  // Load the grammar
  Grammar file = process(file_name);
  // Get an iterator to the file contents.
  std::list<std::string>::iterator line = file.contents.begin();
  bool first_production = true;
  // Iterate
  do {
    // Get current line
    std::string curr = *line;
    // Removed newline
    curr.erase(std::remove(curr.begin(), curr.end(), '\n'), curr.end());
    // If delimiter, done reading terminals. Break.
    if (curr == DELIM) break;
    // Otherwise, add symbol to set of terminals
    _terminals.insert(curr[0]);
    // Increment iterator
    ++line;
  } while (true);
  do {
    // Icrement iterator (put here to make sure DELIM line is skipped)
    ++line;
    // Get current line
    std::string curr = *line;
    // Remove newline
    curr.erase(std::remove(curr.begin(), curr.end(), '\n'), curr.end());
    // If delimiter, done reading productions. Break.
    if (curr == DELIM) break;
    // If it's the first one, add delimiter to first of LHS
    if (first_production) {
      _follow[curr[0]].insert(DELIM[0]);
      first_production = false;
    }
    // Add LHS to set of non-terminals
    _non_terminals.insert(curr[0]);
    // Add whole line to set of productions
    _productions.insert(*line);
  } while (true);
  for (auto item : _terminals)     std::cout << item << std::endl;
  for (auto item : _non_terminals) std::cout << item << std::endl;
  for (auto item : _productions)   std::cout << item;
}


/**
 * Finds the first and follow sets for the Parser
 * @return exit code
 */
void Parser::parse() {
  findFirst();
  findFollow();
}


/**
 * Returns the first sets for the Parser
 * @return the first sets
 */
std::map<char, std::set<char>> Parser::first() const {
  return _first;
}


/**
 * Returns the follow sets for the Parser
 * @return the follow sets
 */
std::map<char, std::set<char>> Parser::follow() const {
  return _follow;
}


/**
 * Run the Preprocessor
 * @param  file_name -> The name of the file to be processed
 * @return the Grammar created
 */
Grammar Parser::process(const std::string file_name) {
  Preprocessor p(file_name);
  return p.run();
}


/**
 * Finds the first set for the Parser
 * @return exit code
 */
void Parser::findFirst() {
  bool changed;
  for (auto terminal : _terminals) {
    _first[terminal].insert(terminal);
  }
  for (auto production : _productions) {
    if (production.substr(3) == EPSILON) {
      _first[production[0]].insert(EPSILON[0]);
    }
  }
  do {
    changed = false;
    for (auto production : _productions) {
      size_t i = 0;
      char lhs = production[0];
      std::string rhs = production.substr(3);
      while (i < rhs.length()) {
        std::set<char> first = _first[rhs[i]];
        if (hasEpsilon(first)) {
          first.erase(first.find(EPSILON[0]));
          addSetToFirst(lhs, first, &changed);
          ++i;
        } else {
          addSetToFirst(lhs, first, &changed);
          break;
        }
        if (i >= rhs.length()) {
          addCharToFirst(lhs, EPSILON[0], &changed);
        }
        ++i;
      }
    }
  } while (changed);
}


/**
 * Find the follow set for each symbol.
 * @return exit code
 */
void Parser::findFollow() {
  bool changed;
  do {
    changed = false;
    for (auto production : _productions) {
      size_t i = 0;
      char lhs = production[0];
      std::string rhs = production.substr(3);
      while (i < rhs.length()) {
        bool is_non_terminal = isNonTerminal(rhs[i]);
        if (is_non_terminal && i < rhs.length() - 1) {
          std::set<char> next_first = _first[rhs[i + 1]];
          bool has_epsilon = next_first.erase(EPSILON[0]);
          addSetToFollow(rhs[i], next_first, &changed);
          if (has_epsilon) {
            addSetToFollow(rhs[i], _follow[lhs], &changed);
          }
        } else if (is_non_terminal && i == rhs.length() - 1) {
          addSetToFollow(rhs[i], _follow[lhs], &changed);
        }
        ++i;
      }
    }
  } while (changed);
}


/**
 * Add the given first set to the first of the given nonterminal
 * @param nonterminal -> The symbol whose first is being added to
 * @param first       -> The symbols being added
 * @param changed     -> A flag to see if anything actually changed
 */
void Parser::addSetToFirst(const char nonterminal,
                           const std::set<char> first,
                           bool *changed) {
  for (auto symbol : first) {
    auto result = _first[nonterminal].insert(symbol);
    if (result.second) *changed = true;
  }
}


/**
 * Adds the given symbol to the first of the given nonterminal
 * @param nonterminal -> The symbol whose first is being added to
 * @param symbol      -> The symbol being added
 * @param changed     -> A flag to see if anything actually changed
 */
void Parser::addCharToFirst(const char nonterminal,
                            const char symbol,
                            bool *changed) {
  auto result = _first[nonterminal].insert(symbol);
  if (result.second) *changed = true;
}


/**
 * Add the given follow set to the first of the given nonterminal
 * @param nonterminal -> The symbol whose follow is being added to
 * @param follow      -> The symbols being added
 * @param changed     -> A flag to see if anything actually changed
 */
void Parser::addSetToFollow(const char nonterminal,
                            const std::set<char> follow,
                            bool *changed) {
  for (auto symbol : follow) {
    auto result = _follow[nonterminal].insert(symbol);
    if (result.second) *changed = true;
  }
}


/**
 * Checks whether epsilon is present in the given set.
 * @param  first -> The set being checked for epsilon
 * @return the result of the test
 */
bool Parser::hasEpsilon(const std::set<char> first) {
  return first.find(EPSILON[0]) != first.end();
}


/**
 * Checks whether the given symbol is a non_terminal
 * @param  symbol -> The symbol being checked
 * @return TRUE if a terminal, FALSE otherwise
 */
bool Parser::isNonTerminal(const char symbol) {
  return _non_terminals.find(symbol) != _non_terminals.end();
}
