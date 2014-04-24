/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef OS_H
#define OS_H

#include <cstdint>
#include <list>
#include <queue>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include "../asm/Assembler.h"
#include "../vm/VirtualMachine.h"

#define N_REGISTERS 4

class OS {
 public:
  OS();

  void run();

 private:
  struct PCB {
    uint16_t pc = 0, sr = 0, sp = 0, base = 0, limit = 0;
    std::vector<int16_t> r;
    std::string pname;
    std::ifstream o_file, in_file;
    std::ofstream out_file, st_file;
    uintmax_t vm_time = 0, waiting_time = 0, turnaround_time = 0, io_time = 0,
              largest_stack_size = 0;
#ifdef DEBUG
    std::vector<std::string> asm_source;
#endif  // DEBUG
  };

  Assembler as;
  VirtualMachine vm;
  std::queue<std::unique_ptr<PCB>> ready, waiting;
  std::unique_ptr<PCB> running;

  std::vector<std::string> get_sourcefiles();
};

#endif  // OS_H
