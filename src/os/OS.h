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
};

#endif  // OS_H
