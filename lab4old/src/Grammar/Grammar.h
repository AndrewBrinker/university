/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <vector>

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

 private:
  bool findFirst();
  bool findFollow();

  std::vector<Symbol> _symbols;
  std::vector<std::string> _productions;

  DISALLOW_COPY_AND_ASSIGN(Grammar);
};

#endif  // GRAMMAR_H
