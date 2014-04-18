/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./VirtualMachine.h"
#include <err/Errors.h>
#include <util/Utilities.h>
#include <asm/Assembler.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>

#ifdef DEBUG
 extern std::vector<std::string> debug_source;
#endif  // DEBUG

#define REG_FILE_SIZE 4
#define MEM_SIZE      256

#define OUT_FILE_EXT ".out"
#define IN_FILE_EXT  ".in"


/**
 * Construct the VM with the default values
 */
VirtualMachine::VirtualMachine()
  : VirtualMachine(REG_FILE_SIZE, MEM_SIZE) {}


/**
 * Construct the VM with custom register size and memory size, and setup the op
 * code for later use.
 */
VirtualMachine::VirtualMachine(uint16_t reg_file_size,
                               uint16_t mem_size)
                             : r(reg_file_size),
                               mem(mem_size),
                               pc(0),
                               sp(mem_size - 1),
                               base(0),
                               clock(0),
                               halt(0) {
  setupOpMap();
}


/**
 * Parse the given file and run the code it describes
 * @param file_name -> The name of the file being parsed
 */
void VirtualMachine::run(std::string file_name) {
  try {
    if (!isFileNameValid(file_name, VM_MODE)) {
      throw InvalidFileName("Virtual Machine");
    }
  } catch(GenericError &e) {
    e.reportError();
  }

  try {
    if (!doesFileExist(file_name)) {
      throw FileDoesNotExist("Virtual Machine");
    }
  } catch(GenericError &e) {
    e.reportError();
  }

  std::ifstream input_file(file_name);
  try {
    if (!input_file.is_open()) {
      throw CantOpenFile("Virtual Machine");
    }
  } catch(GenericError &e) {
    e.reportError();
  }

  std::string object_file_name = stripExtension(file_name);
  object_file_name += OUT_FILE_EXT;
  dot_out_file.open(object_file_name);
  try {
    if (!dot_out_file.is_open()) {
      throw CantMakeFile("Virtual Machine");
    }
  } catch(GenericError &e) {
    e.reportError();
  }

  object_file_name = stripExtension(file_name);
  object_file_name += IN_FILE_EXT;
  dot_in_file.open(object_file_name);

  // Get size of file
  input_file.seekg(0, std::ios::end);
  limit = input_file.tellg();
  input_file.seekg(0, std::ios::beg);

  // because size will be in bytes, not in 16-bit words
  if (limit > mem.size() * 2) {
    input_file.close();
    try {
      throw CantFitInMemory("Virtual Machine");
    } catch(GenericError &e) {
      e.reportError();
    }
    return;
  }

  // Load file into memory
  int count = 0;
  for (std::string line; getline(input_file, line);) {
    mem[count] = stoi(line);
    ++count;
  }

  input_file.close();

#ifdef DEBUG
  std::ofstream log_file(stripExtension(file_name) + ".log");
#endif  // DEBUG

  try {
    // main loop
    while (!halt) {
#ifdef DEBUG
      log_file << debug_source[pc] << std::endl;
#endif  // DEBUG
      ir.i = mem[pc];
      ++pc;
      (*this.*ops[ir.i >> 8])();
      clock += clocks[ir.i >> 8];
#ifdef DEBUG
      log_file << "r0: " << r[0] << ' ';
      log_file << "r1: " << r[1] << ' ';
      log_file << "r2: " << r[2] << ' ';
      log_file << "r3: " << r[3] << std::endl;
      for (unsigned int i = 0; i < mem.size(); ++i) {
        log_file << hex(mem[i] & 0xffff, 4) << ' ';
        if (i % 16 == 15) log_file << std::endl;
      }
      log_file << std::endl << std::endl;
#endif  // DEBUG
    }
  } catch(std::bad_function_call&) {
    try {
      throw InvalidOperation("Virtual Machine");
    } catch(GenericError &e) {
      e.reportError();
    }
  }

  dot_out_file << clock << std::endl;
  dot_in_file.close();
  dot_out_file.close();
#ifdef DEBUG
  log_file.close();
#endif  // DEBUG
}


/**
 * Test the value of the overflow bit.
 * @return whether the overflow bit is 1 or 0.
 */
inline bool VirtualMachine::bt_overflow() const {
  return sr & 0x0010;
}


/**
 * Test the value of the overflow bit and set it to 0.
 * @return whether the overflow bit is 1 or 0.
 */
inline bool VirtualMachine::btr_overflow() {
  bool s = sr & 0x0010;
  sr &= 0xFFEF;
  return s;
}


/**
 * Test the value of the given bit and set it to 1.
 * @return whether the overflow bit is 1 or 0.
 */
inline bool VirtualMachine::bts_overflow() {
  bool s = sr & 0x0010;
  sr |= 0x0010;
  return s;
}


/**
 * Test the value of the less bit.
 * @return whether the less bit is 1 or 0.
 */
inline bool VirtualMachine::bt_less() const {
  return sr & 0x0008;
}


/**
 * Test the value of the less bit and set it to 0.
 * @return whether the less bit is 1 or 0.
 */
inline bool VirtualMachine::btr_less() {
  bool s = sr & 0x0008;
  sr &= 0xFFF7;
  return s;
}


/**
 * Test the value of the less bit and set it to 1.
 * @return whether the less bit is 1 or 0.
 */
inline bool VirtualMachine::bts_less() {
  bool s = sr & 0x0008;
  sr |= 0x0008;
  return s;
}


/**
 * Test the value of the equal bit.
 * @return whether the equal bit is 1 or 0.
 */
inline bool VirtualMachine::bt_equal() const {
  return sr & 0x0004;
}


/**
 * Test the value of the equal bit and set it to 0.
 * @return whether the equal bit is 1 or 0.
 */
inline bool VirtualMachine::btr_equal() {
  bool s = sr & 0x0004;
  sr &= 0xFFFB;
  return s;
}


/**
 * Test the value of the equal bit and set it to 1.
 * @return whether the equal bit is 1 or 0.
 */
inline bool VirtualMachine::bts_equal() {
  bool s = sr & 0x0004;
  sr |= 0x0004;
  return s;
}


/**
 * Test the value of the greater bit.
 * @return whether the greater bit is 1 or 0.
 */
inline bool VirtualMachine::bt_greater() const {
  return sr & 0x0002;
}


/**
 * Test the value of the greater bit and set it to 0.
 * @return whether the greater bit is 1 or 0.
 */
inline bool VirtualMachine::btr_greater() {
  bool s = sr & 0x0002;
  sr &= 0xFFFD;
  return s;
}


/**
 * Test the value of the greater bit and set it to 1.
 * @return whether the greater bit is 1 or 0.
 */
inline bool VirtualMachine::bts_greater() {
  bool s = sr & 0x0002;
  sr |= 0x0002;
  return s;
}


/**
 * Test the value of the carry bit.
 * @return whether the carry bit is 1 or 0.
 */
inline bool VirtualMachine::bt_carry() const {
  return sr & 0x0001;
}


/**
 * Test the value of the carry bit and set it to 0.
 * @return whether the carry bit is 1 or 0.
 */
inline bool VirtualMachine::btr_carry() {
  bool s = sr & 0x0001;
  sr &= 0xFFFE;
  return s;
}


/**
 * Test the value of the carry bit and set it to 1.
 * @return whether the carry bit is 1 or 0.
 */
inline bool VirtualMachine::bts_carry() {
  bool s = sr & 0x0001;
  sr |= 0x0001;
  return s;
}


/**
 * Load the given memory location's value into a register.
 */
void VirtualMachine::op_load() {
  r[ir.fmt1.rd] = mem[ir.fmt1.addr];
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
  mem[ir.fmt1.addr] = r[ir.fmt1.rd];
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
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt0.rd] = temp & 0xffff;
}


/**
 * Add the given constant to rd and store in rd, accounting for carries.
 */
void VirtualMachine::op_addi() {
  int32_t temp = r[ir.fmt1.rd];
  temp += ir.fmt1.constant;
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt1.rd] = temp & 0xffff;
}


/**
 * Add rs to rd and store in rd, incrementing temp afterward.
 */
void VirtualMachine::op_addc() {
  int32_t temp = r[ir.fmt0.rd];
  temp += r[ir.fmt0.rs];
  ++temp;
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt0.rd] = temp & 0xffff;
}


/**
 * Add the constant to rd and store in rd, incrementing temp afterward.
 */
void VirtualMachine::op_addci() {
  int32_t temp = r[ir.fmt1.rd];
  temp += ir.fmt1.constant;
  ++temp;
  if (temp & 0x00010000) bts_carry();
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
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt0.rd] = temp & 0xffff;
}


/**
 * Subtract constant from rd and store in rd.
 */
void VirtualMachine::op_subi() {
  int32_t temp = r[ir.fmt1.rd];
  temp -= ir.fmt1.constant;
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt0.rd] = temp & 0xffff;
}


/**
 * Substract rs from rd and store in rd, decrementing temp afterward.
 */
void VirtualMachine::op_subc() {
  int32_t temp = r[ir.fmt0.rd];
  temp -= r[ir.fmt0.rs];
  if (bts_carry()) --temp;
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt0.rd] = temp & 0xffff;
}


/**
 * Subtract constant from rd and store in rd, decrementing temp afterward.
 */
void VirtualMachine::op_subci() {
  int32_t temp = r[ir.fmt1.rd];
  temp -= ir.fmt1.constant;
  --temp;
  if (bts_carry()) --temp;
  if (temp & 0x00010000) bts_carry();
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
  if (r[ir.fmt0.rd] & 0x80) bts_carry();
  r[ir.fmt0.rd] <<= 1;
}


/**
 * Take arithmetic left shift of rd, storing in rd.
 *
 * Retains sign bit.
 */
void VirtualMachine::op_shla() {
  if (r[ir.fmt0.rd] & 0x80) bts_carry();
  r[ir.fmt0.rd] = (r[ir.fmt0.rd] & 0x80) | ((r[ir.fmt0.rd] << 1) & 0x7f);
}


/**
 * Take logical right shift of rd, storing in rd.
 *
 * Does not retain sign bit.
 */
void VirtualMachine::op_shr() {
  if (r[ir.fmt0.rd] & 0x01) bts_carry();
  r[ir.fmt0.rd] = (r[ir.fmt0.rd] >> 1) & 0x7f;
}


/**
 * Take arithmetic right shift of rd, storing in rd.
 *
 * Retains sign bit.
 */
void VirtualMachine::op_shra() {
  if (r[ir.fmt0.rd] & 0x01) bts_carry();
  r[ir.fmt0.rd] >>= 1;
}


/**
 * Compare rs and rd, setting the less, greater, and equal bits
 */
void VirtualMachine::op_compr() {
  if (r[ir.fmt0.rd] < r[ir.fmt0.rs]) {
    bts_less(), btr_equal(), btr_greater();
  } else if (r[ir.fmt0.rd] == r[ir.fmt0.rs]) {
    btr_less(), bts_equal(), btr_greater();
  } else {
    btr_less(), btr_equal(), bts_greater();
  }
}


/**
 * Compare rd and constant, setting the less, greater, and equal bits
 */
void VirtualMachine::op_compri() {
  if (r[ir.fmt1.rd] < ir.fmt1.constant) {
    bts_less(), btr_equal(), btr_greater();
  } else if (r[ir.fmt1.rd] == ir.fmt1.constant) {
    btr_less(), bts_equal(), btr_greater();
  } else {
    btr_less(), btr_equal(), bts_greater();
  }
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
  if (bt_less()) pc = ir.fmt1.addr;
}


/**
 * Set pc to the given address if equal is true
 */
void VirtualMachine::op_jumpe() {
  if (bt_equal()) pc = ir.fmt1.addr;
}


/**
 * Set pc to the given address if greater is true
 */
void VirtualMachine::op_jumpg() {
  if (bt_greater()) pc = ir.fmt1.addr;
}


/**
 * Call the given function, saving state appropriately
 */
void VirtualMachine::op_call() {
  if (sp < limit + 6) {
    try {
      throw ExceededMaxStackDepth("Virtual Machine");
    } catch(GenericError &e) {
      e.reportError();
    }
  }
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
}


/**
 * Return from the given function and restore state
 */
void VirtualMachine::op_return() {
  if (sp >= 256) {
    try {
      throw StackUnderflow("Virtual Machine");
    } catch(GenericError &e) {
      e.reportError();
    }
  }
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
}


/**
 * Read data from input file into rd
 */
void VirtualMachine::op_read() {
  dot_in_file >> r[ir.fmt0.rd];
}


/**
 * Write data from rd out to output file
 */
void VirtualMachine::op_write() {
  dot_out_file << r[ir.fmt0.rd] << std::endl;
}


/**
 * Set halt to true
 */
void VirtualMachine::op_halt() {
  halt = true;
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
