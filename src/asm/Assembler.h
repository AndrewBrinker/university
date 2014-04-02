/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <vector>
#include <list>
#include <string>
#include <cstdint>

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

/*
typedef void (*func)(int, int);

typedef struct instruction {
  int id;
  func op;
} instruction;
*/

class Assembler {
 public:
  Assembler();
  //std::list<uint16_t> parse(char *);

 private:
  DISALLOW_COPY_AND_ASSIGN(Assembler);

  // const std::vector<instruction> ops;
};


#endif  // ASSEMBLER_H
