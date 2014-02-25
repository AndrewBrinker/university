#ifndef SYMBOL_H
#define SYMBOL_H

class Symbol {
 public:
  Symbol(char, bool);

  char name;
  std::set<char> first;
  set::set<char> follow;

  bool is_terminal;
};

#endif  // SYMBOL_H