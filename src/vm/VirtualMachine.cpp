/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./VirtualMachine.h"
#include <cstdint>

#define REG_FILE_SIZE 4
#define MEM_SIZE      256

/**
 * Default constructor
 *
 * Initializes internal variables to their default values.
 */
VirtualMachine::VirtualMachine()
  : VirtualMachine(REG_FILE_SIZE, MEM_SIZE) {}


/**
 * Alternate constructor
 *
 * Initializes internal variables to user-defined values.
 *
 * @param reg_file_size -> The size of the register
 * @param mem_size      -> The size of memory
 */
VirtualMachine::VirtualMachine(uint16_t reg_file_size,
                               uint16_t mem_size) :
                               r(reg_file_size),
                               mem(mem_size) {}
