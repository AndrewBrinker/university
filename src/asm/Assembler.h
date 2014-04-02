/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class Assembler {
 public:
  Assembler();

 private:
  DISALLOW_COPY_AND_ASSIGN(Assembler);

};

#endif  // ASSEMBLER_H
