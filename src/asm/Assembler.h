/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <cstdint>
#include <vector>
#include <list>
#include <string>

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);


class Assembler {
 public:
  typedef std::list<std::string> ASMSource;

  Assembler();
  std::string parse(std::string);

 private:
  bool isFileNameValid(std::string);
  bool doesFileExist(std::string);

  void reportError(std::exception &);
  std::string stripExtension(std::string);
  ASMSource readASMSource(std::ifstream &);

  DISALLOW_COPY_AND_ASSIGN(Assembler);
};

#endif  // ASSEMBLER_H
