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
    uint16_t pc = 0;
    uint16_t sr = 0;
    uint16_t sp = 0;
    uint16_t base = 0;
    uint16_t limit = 0;
    std::vector<int16_t> r;
    std::string pname;
    std::ifstream o_file;
    std::ifstream in_file;
    std::ofstream out_file;
    std::ofstream st_file;
    uintmax_t vm_time = 0;
    uintmax_t waiting_time = 0;
    uintmax_t turnaround_time = 0;
    uintmax_t io_time = 0;
    uintmax_t largest_stack_size = 0;
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
