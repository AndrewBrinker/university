/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./VirtualMachine.h"
#include <algorithm>

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

/**
 * Loads program into memory and runs it
 *
 * This function assumes that the program to be loaded is no longer than
 * REG_FILE_SIZE; if this assumption is not met, then the instructions
 * will be truncated.
 * 
 * @param prgm -> The program
 */
void VirtualMachine::run(std::list<uint16_t> prgm)
{
  std::copy(mem.begin(), mem.end(), prgm.begin());
}

void VirtualMachine::load(uint8_t rd, uint8_t addr)
{
  r[rd] = mem[addr];
}

void VirtualMachine::loadi(uint8_t rd, uint8_t c)
{
  r[rd] = c;
}

void VirtualMachine::store(uint8_t rd, uint8_t addr)
{
  mem[addr] = r[rd];
}

void VirtualMachine::add(uint8_t rd, uint8_t rs)
{
  if( r[rd] & 0x7000 || 
  r[rd] += r[rs];

