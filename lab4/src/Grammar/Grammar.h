/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <set>
#include <map>

#ifndef GRAMMAR_H
#define GRAMMAR_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

class Grammar {
 public:
  explicit Grammar(std::string);
  int parse();

  // Accessor methods for FIRST and FOLLOW
  std::map<char, std::set<char>> first() const;
  std::map<char, std::set<char>> follow() const;

 private:
  // Wrapper methods for finding FIRST and FOLLOW
  bool findFirst();
  bool findFollow();

  // Assistance methods for finding FIRST
  void firstForTerminals();
  void firstForEpsilonProductions();
  void firstForNonterminals();

  // Assistance methods for updating FIRST
  void addSetToFirst(char, std::set<char>, bool *);
  void addCharToFirst(char, char, bool *);

  // Checks whether the given set has epsilon
  bool hasEpsilon(std::set<char>);

  // Member variables
  std::set<char>                 _terminals;
  std::set<char>                 _non_terminals;
  std::set<std::string>          _productions;
  std::map<char, std::set<char>> _first;
  std::map<char, std::set<char>> _follow;

  DISALLOW_COPY_AND_ASSIGN(Grammar);
};

#endif  // GRAMMAR_H
