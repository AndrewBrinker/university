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
  typedef struct op_t {
    std::string name;
    std::string op_code;
    std::string i;
    int format;
  } op_t;

  Assembler();
  std::string parse(std::string);

 private:
  std::vector<std::string> readASMSource(std::ifstream &);
  std::string convertToObjectCode(std::string);
  op_t findOperation(std::string);
  std::string getRegisterID(std::string);

  DISALLOW_COPY_AND_ASSIGN(Assembler);
};

#endif  // ASSEMBLER_H
