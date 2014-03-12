/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <set>
#include <map>
#include "./Grammar.h"
#include "./Item.h"
#include "./LRSet.h"

#ifndef PARSER_H
#define PARSER_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class Parser {
 public:
  explicit Parser(const std::string);
  void parse();
  std::map<char, std::set<char>> first() const;
  std::map<char, std::set<char>> follow() const;
  std::set<LRSet> canon() const;

 private:
  Grammar process(const std::string);
  void findFirst();
  void findFollow();
  void findCanonicalSet();

  std::set<Item> findClosure(std::set<Item>);
  std::set<Item> findGoto(std::set<Item>, char);

  void addSetToFirst(const char, const std::set<char>, bool *);
  void addCharToFirst(const char, const char, bool *);
  void addSetToFollow(const char, const std::set<char>, bool *);

  bool hasEpsilon(const std::set<char>);
  bool isNonTerminal(const char);
  bool isTerminal(const char);
  bool isSubset(std::set<Item>, std::set<Item>);
  std::string getStartProduction();

  std::set<char> _terminals;
  std::set<char> _non_terminals;
  std::set<std::string> _productions;
  std::map<char, std::set<char>> _first;
  std::map<char, std::set<char>> _follow;
  std::set<LRSet> _canon;

  friend std::set<char> setUnion(std::set<char> s1, std::set<char> s2);

  DISALLOW_COPY_AND_ASSIGN(Parser);
};

#endif  // PARSER_H
