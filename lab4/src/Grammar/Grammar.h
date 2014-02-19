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
  Grammar();
  int load(std::string);
  int parse();

  std::map<char, std::set<char>> first() const;
  std::map<char, std::set<char>> follow() const;

 private:
  bool findFirst();
  bool findFollow();

  std::set<char> _terminals;
  std::set<char> _non_terminals;
  std::set<std::string> _productions;

  std::map<char, std::set<char>> _first;
  std::map<char, std::set<char>> _follow;

  DISALLOW_COPY_AND_ASSIGN(Grammar);
};

#endif  // GRAMMAR_H
