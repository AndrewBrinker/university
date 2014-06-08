/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <pcb/PCB.h>
#include <cstdint>
#include <vector>
#include <string>

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete;
#endif

class Assembler {
 public:
  typedef struct op_t {
    std::string name;
    std::string op_code;
    std::string i;
    int format;
  } op_t;

  Assembler();
  void parse(PCB* pcb);

 private:
  std::vector<std::string> readASMSource(std::fstream&);
  std::string              convertToObjectCode(std::string);
  op_t                     findOperation(std::string);
  std::string              getRegisterID(std::string);

  DISALLOW_COPY_AND_ASSIGN(Assembler)
};

#endif  // ASSEMBLER_H
