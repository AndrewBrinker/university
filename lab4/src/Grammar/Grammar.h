/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <vector>
#include <set>

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

  std::vector<std::set<std::string>> first() const;
  std::vector<std::set<std::string>> follow() const;

 private:
  bool getFirst();
  bool getFollow();

  std::vector<std::set<std::string>> _first;
  std::vector<std::set<std::string>> _follow;

  std::string _name;
  std::vector<std::string> _terminals;
  std::vector<std::string> _productions;

  DISALLOW_COPY_AND_ASSIGN(Grammar);
};

#endif  // GRAMMAR_H
