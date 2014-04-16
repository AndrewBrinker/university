/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <cstdint>
#include <vector>
#include <string>

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class Assembler {
 public:
  typedef std::vector<std::string> ASMSource;
  typedef std::vector<std::string> ObjectSource;

  typedef struct op {
    std::string name;
    std::string op_code;
    std::string i;
    int format;
  } op;

  Assembler();
  std::string parse(std::string);

 private:
  bool isFileNameValid(std::string);
  bool doesFileExist(std::string);

  void reportError(std::exception &);
  std::string stripExtension(std::string);
  std::string stripComments(std::string);
  std::string stripEndingWhitespace(std::string);
  std::vector<std::string> split(std::string);
  ASMSource readASMSource(std::ifstream &);
  std::string convertToObjectCode(std::string);
  op findOperation(std::string);
  std::string getRegisterID(std::string);
  void pad(std::string *, const char, size_t);
  std::string toSignedBinaryString(const int, const int);
  std::string toUnsignedBinaryString(const int, const int);

  DISALLOW_COPY_AND_ASSIGN(Assembler);
};

#endif  // ASSEMBLER_H
