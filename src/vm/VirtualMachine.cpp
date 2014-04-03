/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./VirtualMachine.h"
#include <cstdint>

#define REG_FILE_SIZE 4
#define MEM_SIZE      256

VirtualMachine::VirtualMachine()
  : VirtualMachine(REG_FILE_SIZE, MEM_SIZE) {}


VirtualMachine::VirtualMachine(uint16_t,
                               uint16_t mem_size) :
                               mem(mem_size) {}


void VirtualMachine::run(Assembler::ObjectSource) {
    return;
}