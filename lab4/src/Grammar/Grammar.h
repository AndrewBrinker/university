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

typedef std::map<char, std::set<char>> mapset;

class Grammar {
 public:
  Grammar();
  int load(std::string);
  int parse();

  mapset first() const;
  mapset follow() const;

 private:
  bool findFirst();
  bool findFollow();

  std::set<char> _terminals;
  std::set<char> _non_terminals;
  std::set<std::string> _productions;

  mapset _first;
  mapset _follow;

  DISALLOW_COPY_AND_ASSIGN(Grammar);
};

#endif  // GRAMMAR_H
