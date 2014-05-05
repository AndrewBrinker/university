/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./VirtualMachine.h"
#include <err/Errors.h>
#include <util/Utilities.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <cassert>


#define OUT_FILE_EXT ".out"
#define IN_FILE_EXT  ".in"

#define OVERFLOW_MASK 0x0010
#define LESS_MASK     0x0008
#define EQUAL_MASK    0x0004
#define GREATER_MASK  0x0002
#define CARRY_MASK    0x0001

#define RETURN_STATUS_MASK  0x7
#define RETURN_STATUS_SHIFT 5
#define IO_REGISTER_MASK    0x3
#define IO_REGISTER_SHIFT   8


/**
 * Construct the VM with custom register size and memory size, and setup the op
 * code for later use.
 */
VirtualMachine::VirtualMachine()
                             : r(REG_FILE_SIZE),
                               mem(MEM_SIZE),
                               pc(0),
                               sp(MEM_SIZE - 1),
                               base(0),
                               limit(0),
                               halt(0) {
  setupOpMap();
}


/**
 * Load the program indicated by the PCB into memory, setting appropriate
 * variables in the PCB.
 * @param pcb -> The Program Control Block
 */
void VirtualMachine::loadIntoMemory(PCB* pcb) {
  pcb->base = limit;
  base = pcb->base;
  loadFile(pcb->o_file);
  pcb->limit = limit - base;
  pcb->o_file.close();
  unloadPCB(pcb);
}


/**
 * Run the process with the given time slice.
 * @param  pcb        -> The process to run.
 * @param  time_slice -> The time slice allocated to it.
 * @return the exit status of the process.
 */
uint8_t VirtualMachine::runProcess(PCB* pcb, uint8_t time_slice) {
  loadPCB(pcb);
  uint8_t count = 0;
  halt = false;
  try {
    while (!halt) {
      ir.i = mem[pc + base];
      ++pc;
      (*this.*ops[ir.i >> 8])();
      if (!halt) {
        pcb->vm_time += clocks[ir.i >> 8];
        count += clocks[ir.i >> 8];
        if (count >= time_slice) {
          halt = true;
          setReturnStatus(ReturnStatus_t::TIME_SLICE);
        }
      }
    }
  } catch(std::bad_function_call&) {
    setReturnStatus(ReturnStatus_t::INVALID_OPCODE);
  }
  unloadPCB(pcb);
  return count;
}


/**
 * Load the given file into memory.
 * @param object_file -> A stream to the file being loaded.
 */
void VirtualMachine::loadFile(std::fstream& object_file) {
  uint16_t new_limit = limit;
  // Get size of file
  object_file.seekg(0, std::ios::end);
  new_limit += object_file.tellg() / 2;
  object_file.seekg(0, std::ios::beg);
  if (new_limit > mem.size()) {
    object_file.close();
    throw CantFitInMemory("VirtualMachine");
  }
  for (std::string line; getline(object_file, line); ++limit) {
    mem[limit] = stoi(line);
  }
}


/**
 * Load the given PCB into the VM.
 * @param pcb -> The PCB being loaded.
 */
void VirtualMachine::loadPCB(PCB* pcb) {
  std::cout << pcb->process_name << std::endl;
  pc = pcb->pc;
  r  = pcb->r;
  sr = pcb->sr;
  sp = pcb->sp;
  base = pcb->base;
  if (sp < MEM_SIZE - 1) {
    readStack(pcb->st_file);
  }
}


/**
 * Write back changes to the PCB.
 * @param pcb -> The PCB being modified.
 */
void VirtualMachine::unloadPCB(PCB* pcb) {
  std::cout << pcb->process_name << std::endl;
  pcb->pc = pc;
  pcb->r = r;
  pcb->sr = sr;
  pcb->sp = sp;
  if (sp < MEM_SIZE - 1) {
    writeStack(pcb->st_file);
  }
}


/**
 * Read in the stack from the given stack file stream.
 * @param stack_file -> The file being read from.
 */
void VirtualMachine::readStack(std::fstream& stack_file) {
  std::cout << "READ SIZE:  " << mem.size() - 1 - sp << "\n\t";
  for (uint16_t i = mem.size() - 1; i != sp; --i) {
    stack_file >> mem[i];
    std::cout << mem[i] << " ";
  }
  std::cout << std::endl << std::endl;
}


/**
 * Write the stack contents out to the given file.
 * @param stack_file -> The file being written to.
 */
void VirtualMachine::writeStack(std::fstream& stack_file) {
  std::cout << "WRITE SIZE: " << mem.size() - 1 - sp << "\n\t";
  for (uint16_t i = mem.size() - 1; i != sp; --i) {
    stack_file << mem[i] << std::endl;
    std::cout << mem[i] << " ";
  }
  std::cout << std::endl;
}


/**
 * Test the value of the overflow bit.
 * @return the value of the overflow bit.
 */
inline bool VirtualMachine::getOverflow() const {
  return sr & OVERFLOW_MASK;
}


/**
 * Test the value of the overflow bit, and set it to the input value.
 * @param The value to which to set the overflow bit
 * @return the value of the overflow bit /before/ modification.
 */
inline bool VirtualMachine::setOverflow(bool b) {
  bool s = sr & OVERFLOW_MASK;
  if (b) {
    sr |= OVERFLOW_MASK;
  } else {
    sr &= ~OVERFLOW_MASK;
  }
  return s;
}


/**
 * Test the value of the less bit.
 * @return the value of the less bit.
 */
inline bool VirtualMachine::getLess() const {
  return sr & LESS_MASK;
}


/**
 * Test the value of the less bit, and set it to the value of the parameter.
 * @param The value to which to set the less bit.
 * @return the value of the less bit /before/ modification.
 */
inline bool VirtualMachine::setLess(bool b) {
  bool s = sr & LESS_MASK;
  if (b) {
    sr |= LESS_MASK;
  } else {
    sr &= ~LESS_MASK;
  }
  return s;
}


/**
 * Test the value of the equal bit.
 * @return the value of the equal bit.
 */
inline bool VirtualMachine::getEqual() const {
  return sr & EQUAL_MASK;
}


/**
 * Test the value of the equal bit and set it to the value of the parameter.
 * @param The value to which to set the equal bit.
 * @return the value of the equal bit /before/ modification.
 */
inline bool VirtualMachine::setEqual(bool b) {
  bool s = sr & EQUAL_MASK;
  if (b) {
    sr |= EQUAL_MASK;
  } else {
    sr &= ~EQUAL_MASK;
  }
  return s;
}


/**
 * Test the value of the greater bit.
 * @return the value of the greater bit.
 */
inline bool VirtualMachine::getGreater() const {
  return sr & GREATER_MASK;
}


/**
 * Test the value of the greater bit and set it to the value of the parameter.
 * @param The value to which to set the greater bit.
 * @return the value of the greater bit /before/ modification.
 */
inline bool VirtualMachine::setGreater(bool b) {
  bool s = sr & GREATER_MASK;
  if (b) {
    sr |= GREATER_MASK;
  } else {
    sr &= ~GREATER_MASK;
  }
  return s;
}

/**
 * Test the value of the carry bit.
 * @return the value of the carry bit.
 */
inline bool VirtualMachine::getCarry() const {
  return sr & CARRY_MASK;
}

/**
 * Test the value of the carry bit and set it to the value of the parameter.
 * @param The value to which to set the carry bit.
 * @return the value of the carry bit /before/ modification.
 */
inline bool VirtualMachine::setCarry(bool b) {
  bool s = sr & CARRY_MASK;
  if (b) {
    sr |= CARRY_MASK;
  } else {
    sr &= ~CARRY_MASK;
  }
  return s;
}


/**
 * Test the value of the return status.
 * @return the value of the return status.
 */
inline uint8_t VirtualMachine::getReturnStatus() const {
  return (sr & (RETURN_STATUS_MASK >> RETURN_STATUS_SHIFT)) >>
         RETURN_STATUS_SHIFT;
}


/**
 * Test the value of the return status and set it to the value of the parameter.
 * @param The value to which to set the return status.
 * @return the value of the return status /before/ modification.
 */
inline uint8_t VirtualMachine::setReturnStatus(uint8_t status) {
#ifdef DEBUG
  assert(status < 8);
#endif  // DEBUG
  uint8_t s = (sr & (RETURN_STATUS_MASK << RETURN_STATUS_SHIFT)) >>
               RETURN_STATUS_SHIFT;
  sr &= ~(RETURN_STATUS_MASK << RETURN_STATUS_SHIFT);
  sr |= status << RETURN_STATUS_SHIFT;
  return s;
}


/**
 * Test the value of the I/O register field.
 * @return the value of the I/O register field.
 */
inline uint8_t VirtualMachine::getIO_Register() const {
  return (sr & IO_REGISTER_MASK) >> IO_REGISTER_SHIFT;
}


/**
 * Test the value of the I/O register field and set it to the value of the
 * parameter.
 * @param The value to which to set the I/O register field.
 * @return the value of the I/O register field /before/ modification.
 */
inline uint8_t VirtualMachine::setIO_Register(uint8_t reg) {
#ifdef DEBUG
  assert(reg < 4);
#endif  // DEBUG
  uint8_t s = (sr & (IO_REGISTER_MASK << IO_REGISTER_SHIFT)) >>
               IO_REGISTER_SHIFT;
  sr &= ~(IO_REGISTER_MASK << IO_REGISTER_SHIFT);
  sr |= reg << IO_REGISTER_SHIFT;
  return s;
}



/**
 * Load the given memory location's value into a register.
 */
void VirtualMachine::op_load() {
  r[ir.fmt1.rd] = mem[base + ir.fmt1.addr];
}


/**
 * Load the given constant into a register.
 */
void VirtualMachine::op_loadi() {
  r[ir.fmt1.rd] = ir.fmt1.constant;
}


/**
 * Store the given register value in memory.
 */
void VirtualMachine::op_store() {
  mem[base + ir.fmt1.addr] = r[ir.fmt1.rd];
}


/**
 * Add the value of rs to rd and store in rd, accounting for carries.
 *
 * To determine carry, we use a larger-size int than necessary, and check the
 * first bit outside the last two bytes.
 */
void VirtualMachine::op_add() {
  int32_t temp = r[ir.fmt0.rd];
  temp += r[ir.fmt0.rs];
  setCarry(temp & 0x00010000);
  r[ir.fmt0.rd] = temp & 0xffff;
}


/**
 * Add the given constant to rd and store in rd, accounting for carries.
 */
void VirtualMachine::op_addi() {
  int32_t temp = r[ir.fmt1.rd];
  temp += ir.fmt1.constant;
  setCarry(temp & 0x00010000);
  r[ir.fmt1.rd] = temp & 0xffff;
}


/**
 * Add rs to rd and store in rd, incrementing temp afterward.
 */
void VirtualMachine::op_addc() {
  int32_t temp = r[ir.fmt0.rd];
  temp += r[ir.fmt0.rs];
  ++temp;
  setCarry(temp & 0x00010000);
  r[ir.fmt0.rd] = temp & 0xffff;
}


/**
 * Add the constant to rd and store in rd, incrementing temp afterward.
 */
void VirtualMachine::op_addci() {
  int32_t temp = r[ir.fmt1.rd];
  temp += ir.fmt1.constant;
  ++temp;
  setCarry(temp & 0x00010000);
  r[ir.fmt1.rd] = temp & 0xffff;
}


/**
 * Subtract rs from rd and store in rd.
 *
 * NOTE: Not sure how to test for carry when subtracting. This may be correct.
 */
void VirtualMachine::op_sub() {
  int32_t temp = r[ir.fmt0.rd];
  temp -= r[ir.fmt0.rs];
  setCarry(temp & 0x00010000);
  r[ir.fmt0.rd] = temp & 0xffff;
}


/**
 * Subtract constant from rd and store in rd.
 */
void VirtualMachine::op_subi() {
  int32_t temp = r[ir.fmt1.rd];
  temp -= ir.fmt1.constant;
  setCarry(temp & 0x00010000);
  r[ir.fmt0.rd] = temp & 0xffff;
}


/**
 * Substract rs from rd and store in rd, decrementing temp afterward.
 */
void VirtualMachine::op_subc() {
  int32_t temp = r[ir.fmt0.rd];
  temp -= r[ir.fmt0.rs];
  if (getCarry()) --temp;
  setCarry(temp & 0x00010000);
  r[ir.fmt0.rd] = temp & 0xffff;
}


/**
 * Subtract constant from rd and store in rd, decrementing temp afterward.
 */
void VirtualMachine::op_subci() {
  int32_t temp = r[ir.fmt1.rd];
  temp -= ir.fmt1.constant;
  if (getCarry()) --temp;
  setCarry(temp & 0x00010000);
  r[ir.fmt1.rd] = temp & 0xffff;
}


/**
 * Perform logical AND of rs and rd, storing in rd.
 */
void VirtualMachine::op_and() {
  r[ir.fmt0.rd] &= r[ir.fmt0.rs];
}


/**
 * Perform logical AND of rd and constant, storing in rd.
 */
void VirtualMachine::op_andi() {
  r[ir.fmt1.rd] &= ir.fmt1.constant;
}


/**
 * Perform logical XOR of rd and rs, storing in rd.
 */
void VirtualMachine::op_xor() {
  r[ir.fmt0.rd] ^= r[ir.fmt0.rs];
}


/**
 * Perform logical XOR of rd and constant, storing in rd.
 */
void VirtualMachine::op_xori() {
  r[ir.fmt0.rd] ^= ir.fmt1.constant;
}


/**
 * Perform logical NOT of rd, storing in rd.
 */
void VirtualMachine::op_compl() {
  r[ir.fmt0.rd] = ~r[ir.fmt0.rd];
}


/**
 * Take logical left shift of rd, storing in rd.
 *
 * Does not retain sign bit,
 */
void VirtualMachine::op_shl() {
  setCarry(r[ir.fmt0.rd] & 0x80);
  r[ir.fmt0.rd] <<= 1;
}


/**
 * Take arithmetic left shift of rd, storing in rd.
 *
 * Retains sign bit.
 */
void VirtualMachine::op_shla() {
  setCarry(r[ir.fmt0.rd] & 0x80);
  r[ir.fmt0.rd] = (r[ir.fmt0.rd] & 0x80) | ((r[ir.fmt0.rd] << 1) & 0x7f);
}


/**
 * Take logical right shift of rd, storing in rd.
 *
 * Does not retain sign bit.
 */
void VirtualMachine::op_shr() {
  setCarry(r[ir.fmt0.rd] & 0x01);
  r[ir.fmt0.rd] = (r[ir.fmt0.rd] >> 1) & 0x7f;
}


/**
 * Take arithmetic right shift of rd, storing in rd.
 *
 * Retains sign bit.
 */
void VirtualMachine::op_shra() {
  setCarry(r[ir.fmt0.rd] & 0x01);
  r[ir.fmt0.rd] >>= 1;
}


/**
 * Compare rs and rd, setting the less, greater, and equal bits
 */
void VirtualMachine::op_compr() {
  setLess(r[ir.fmt0.rd] < r[ir.fmt0.rs]);
  setEqual(r[ir.fmt0.rd] == r[ir.fmt0.rs]);
  setGreater(r[ir.fmt0.rd] >  r[ir.fmt0.rs]);
}


/**
 * Compare rd and constant, setting the less, greater, and equal bits
 */
void VirtualMachine::op_compri() {
  setLess(r[ir.fmt1.rd] < ir.fmt1.constant);
  setEqual(r[ir.fmt1.rd] == ir.fmt1.constant);
  setGreater(r[ir.fmt1.rd] >  ir.fmt1.constant);
}


/**
 * Load sr into rd.
 */
void VirtualMachine::op_getstat() {
  r[ir.fmt0.rd] = sr;
}


/**
 * Put rd into sr
 */
void VirtualMachine::op_putstat() {
  sr = r[ir.fmt0.rd];
}


/**
 * Set pc to the given address
 */
void VirtualMachine::op_jump() {
  pc = ir.fmt1.addr;
}


/**
 * Set pc to the given address if less is true
 */
void VirtualMachine::op_jumpl() {
  if (getLess()) pc = ir.fmt1.addr;
}


/**
 * Set pc to the given address if equal is true
 */
void VirtualMachine::op_jumpe() {
  if (getEqual()) pc = ir.fmt1.addr;
}


/**
 * Set pc to the given address if greater is true
 */
void VirtualMachine::op_jumpg() {
  if (getGreater()) pc = ir.fmt1.addr;
}


/**
 * Call the given function, saving state appropriately
 */
void VirtualMachine::op_call() {
  if (sp < limit + 6) {
    setReturnStatus(ReturnStatus_t::STACK_OVERFLOW);
    halt = true;
    return;
  }

#ifdef DEBUG
  uint16_t old_sp = sp;
#endif  // DEBUG

  mem[sp] = pc;
  --sp;
  mem[sp] = r[0];
  --sp;
  mem[sp] = r[1];
  --sp;
  mem[sp] = r[2];
  --sp;
  mem[sp] = r[3];
  --sp;
  mem[sp] = sr;
  --sp;
  pc = ir.fmt1.addr;

#ifdef DEBUG
  assert(old_sp - 6 == sp);
#endif  // DEBUG
}


/**
 * Return from the given function and restore state
 */
void VirtualMachine::op_return() {
  if (sp >= 256) {
    setReturnStatus(ReturnStatus_t::STACK_UNDERFLOW);
    halt = true;
    return;
  }

#ifdef DEBUG
  uint16_t old_sp = sp;
#endif  // DEBUG

  ++sp;
  sr = mem[sp];
  ++sp;
  r[3] = mem[sp];
  ++sp;
  r[2] = mem[sp];
  ++sp;
  r[1] = mem[sp];
  ++sp;
  r[0] = mem[sp];
  ++sp;
  pc = mem[sp];

#ifdef DEBUG
  assert(old_sp + 6 == sp);
#endif  // DEBUG
}


/**
 * Read data from input file into rd
 */
void VirtualMachine::op_read() {
//  dot_in_file >> r[ir.fmt0.rd];
  setReturnStatus(ReturnStatus_t::READ_OPERATION);
  setIO_Register(ir.fmt0.rd);
  halt = true;
}


/**
 * Write data from rd out to output file
 */
void VirtualMachine::op_write() {
//  dot_out_file << r[ir.fmt0.rd] << std::endl;
  setReturnStatus(ReturnStatus_t::WRITE_OPERATION);
  setIO_Register(ir.fmt0.rd);
  halt = true;
}


/**
 * Set halt to true
 */
void VirtualMachine::op_halt() {
  halt = true;
  setReturnStatus(ReturnStatus_t::HALT_INSTRUCTION);
}


/**
 * Do nothing
 */
void VirtualMachine::op_noop() {}


/**
 * Load map of opcodes to operation functions
 */
void VirtualMachine::setupOpMap() {
  Opcode_t value;
  for (int i = 0; i < 256; ++i) {
    value.i = i << 8;
    switch (value.fmt0.op) {
    case 0:
      if (value.fmt0.i == 0) {
        clocks[i] = 4;
        ops[i] = &VirtualMachine::op_load;
      } else {
        clocks[i] = 1;
        ops[i] = &VirtualMachine::op_loadi;
      }
      break;
    case 1:
      clocks[i] = 4;
      ops[i] = &VirtualMachine::op_store;
      break;
    case 2:
      clocks[i] = 1;
      ops[i] = (value.fmt0.i == 0)
                ? &VirtualMachine::op_add
                : &VirtualMachine::op_addi;
      break;
    case 3:
      clocks[i] = 1;
      ops[i] = (value.fmt0.i == 0)
                ? &VirtualMachine::op_addc
                : &VirtualMachine::op_addci;
      break;
    case 4:
      clocks[i] = 1;
      ops[i] = (value.fmt0.i == 0)
                ? &VirtualMachine::op_sub
                : &VirtualMachine::op_subi;
      break;
    case 5:
      clocks[i] = 1;
      ops[i] = (value.fmt0.i == 0)
                ? &VirtualMachine::op_subc
                : &VirtualMachine::op_subci;
      break;
    case 6:
      clocks[i] = 1;
      ops[i] = (value.fmt0.i == 0)
                ? &VirtualMachine::op_and
                : &VirtualMachine::op_andi;
      break;
    case 7:
      clocks[i] = 1;
      ops[i] = (value.fmt0.i == 0)
                ? &VirtualMachine::op_xor
                : &VirtualMachine::op_xori;
      break;
    case 8:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_compl;
      break;
    case 9:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_shl;
      break;
    case 10:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_shla;
      break;
    case 11:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_shr;
      break;
    case 12:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_shra;
      break;
    case 13:
      clocks[i] = 1;
      ops[i] = (value.fmt0.i == 0)
                ? &VirtualMachine::op_compr
                : &VirtualMachine::op_compri;
      break;
    case 14:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_getstat;
      break;
    case 15:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_putstat;
      break;
    case 16:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_jump;
      break;
    case 17:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_jumpl;
      break;
    case 18:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_jumpe;
      break;
    case 19:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_jumpg;
      break;
    case 20:
      clocks[i] = 4;
      ops[i] = &VirtualMachine::op_call;
      break;
    case 21:
      clocks[i] = 4;
      ops[i] = &VirtualMachine::op_return;
      break;
    case 22:
      clocks[i] = 28;
      ops[i] = &VirtualMachine::op_read;
      break;
    case 23:
      clocks[i] = 28;
      ops[i] = &VirtualMachine::op_write;
      break;
    case 24:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_halt;
      break;
    case 25:
      clocks[i] = 1;
      ops[i] = &VirtualMachine::op_noop;
      break;
    default:
      clocks[i] = 0;
      /**
       * std::function has a default construction, so there's no need to assign
       * an empty lambda here. Attempting to call any undefined operation will
       * result in std::bad_function_call being thrown.
       */
      break;
    }
  }
}
