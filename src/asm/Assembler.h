/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

#define OP_COUNT 10


typedef void (*func)(int, int);

typedef struct instruction {
  int id;
  func op;
} instruction;


class Assembler {
 public:
  Assembler();

 private:
  DISALLOW_COPY_AND_ASSIGN(Assembler);

  const instruction ops[OP_COUNT];
};


#endif  // ASSEMBLER_H
