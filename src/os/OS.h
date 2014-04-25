/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef OS_H
#define OS_H

#include <asm/Assembler.h>
#include <vm/VirtualMachine.h>
#include <pcb/PCB.h>
#include <cstdint>
#include <list>
#include <queue>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete;
#endif

#define N_REGISTERS 4

class OS {
 public:
  OS();

  void run();

 private:
  std::unique_ptr<Assembler> as;
  std::unique_ptr<VirtualMachine> vm;
  std::queue<std::unique_ptr<PCB>> ready, waiting;
  std::unique_ptr<PCB> running;

  std::vector<std::string> get_sourcefiles();

  DISALLOW_COPY_AND_ASSIGN(OS)
};

#endif  // OS_H
