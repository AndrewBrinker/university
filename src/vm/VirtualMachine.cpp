/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./VirtualMachine.h"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <iterator>

#define REG_FILE_SIZE 4
#define MEM_SIZE      256

VirtualMachine::VirtualMachine()
  : VirtualMachine(REG_FILE_SIZE, MEM_SIZE)
{}


VirtualMachine::VirtualMachine(uint16_t reg_file_size,
                               uint16_t mem_size) :
                              r(reg_file_size),
                              mem(mem_size),
                              pc(0),
                              sp(mem_size),
                              base(0),
                              halt(0) {
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
      ops[i] = (value.fmt0.i == 0) ? &VirtualMachine::op_add : &VirtualMachine::op_addi;
      break;
    case 3:
      clocks[i] = 1;
      ops[i] = (value.fmt0.i == 0) ? &VirtualMachine::op_addc : &VirtualMachine::op_addci;
      break;
    case 4:
      clocks[i] = 1;
      ops[i] = (value.fmt0.i == 0) ? &VirtualMachine::op_sub : &VirtualMachine::op_subi;
      break;
    case 5:
      clocks[i] = 1;
      ops[i] = (value.fmt0.i == 0) ? &VirtualMachine::op_subc : &VirtualMachine::op_subci;
      break;
    case 6:
      clocks[i] = 1;
      ops[i] = (value.fmt0.i == 0) ? &VirtualMachine::op_and : &VirtualMachine::op_andi;
      break;
    case 7:
      clocks[i] = 1;
      ops[i] = (value.fmt0.i == 0) ? &VirtualMachine::op_xor : &VirtualMachine::op_xori;
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
      ops[i] = (value.fmt0.i == 0) ? &VirtualMachine::op_compr : &VirtualMachine::op_compri;
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

void VirtualMachine::run(std::string inFilename) {
  // For op_read and op_write, we need to know the base filename.
  auto it_period = std::find(
    std::begin(inFilename),
    std::end(inFilename),
    '.');
  if (it_period == std::end(inFilename)) {
    fprintf(stderr, "Sorry! Object file must end with \".o.\"\n");
    return;
  }
  std::copy(
    std::begin(inFilename),
    it_period,
    std::back_inserter(base_filename));

  std::ifstream inFile(inFilename, std::ios::binary);

  if (!inFile.is_open()) {
    fprintf(stderr, "Cannot open file.\n");
    exit(1);
  }

  // Get size of file
  inFile.seekg(0, std::ios::end);
  limit = inFile.tellg();
  inFile.seekg(0, std::ios::beg);

  // because size will be in bytes, not in 16-bit words
  if (limit > mem.size() * 2) {
    fprintf(stderr, "Program is too large to fit into memory.\n");
    inFile.close();
    return;
  }

  // Load file into memory
  std::copy_n(
    std::istreambuf_iterator<char>(inFile),
    limit,
    reinterpret_cast<char*>(mem.data()));

  inFile.close();

  dot_in_file.open(base_filename + ".in");
  dot_out_file.open(base_filename + ".out");

  try {
    // main loop
    while (!halt) {
      ir.i = mem[pc];
      ++pc;
      (*this.*ops[ir.i >> 8])();
      clock += clocks[ir.i >> 8];
    }
  } catch (std::bad_function_call&) {
    fprintf(stderr, "Bad operation; must be an assembler bug!\n");
    exit(1);
  } 

  dot_out_file << clock << std::endl;
  dot_in_file.close();
  dot_out_file.close();
}

inline bool VirtualMachine::bt_overflow() const {
  return sr & 0x0010;
}

inline bool VirtualMachine::btr_overflow() {
  bool s = sr & 0x0010;
  sr &= 0xFFEF;
  return s;
}

inline bool VirtualMachine::bts_overflow() {
  bool s = sr & 0x0010;
  sr |= 0x0010;
  return s;
}

inline bool VirtualMachine::bt_less() const {
  return sr & 0x0008;
}

inline bool VirtualMachine::btr_less() {
  bool s = sr & 0x0008;
  sr &= 0xFFF7;
  return s;
}

inline bool VirtualMachine::bts_less() {
  bool s = sr & 0x0008;
  sr |= 0x0008;
  return s;
}

inline bool VirtualMachine::bt_equal() const {
  return sr & 0x0004;
}

inline bool VirtualMachine::btr_equal() {
  bool s = sr & 0x0004;
  sr &= 0xFFFB;
  return s;
}

inline bool VirtualMachine::bts_equal() {
  bool s = sr & 0x0004;
  sr |= 0x0004;
  return s;
}

inline bool VirtualMachine::bt_greater() const {
  return sr & 0x0002;
}

inline bool VirtualMachine::btr_greater() {
  bool s = sr & 0x0002;
  sr &= 0xFFFD;
  return s;
}

inline bool VirtualMachine::bts_greater() {
  bool s = sr & 0x0002;
  sr |= 0x0002;
  return s;
}

inline bool VirtualMachine::bt_carry() const {
  return sr & 0x0001;
}

inline bool VirtualMachine::btr_carry() {
  bool s = sr & 0x0001;
  sr &= 0xFFFE;
  return s;
}

inline bool VirtualMachine::bts_carry() {
  bool s = sr & 0x0001;
  sr |= 0x0001;
  return s;
}

void VirtualMachine::op_load() {
  r[ir.fmt1.rd] = mem[ir.fmt1.addr];
}

void VirtualMachine::op_loadi() {
  r[ir.fmt1.rd] = ir.fmt1.constant;
}

void VirtualMachine::op_store() {
  mem[ir.fmt1.addr] = r[ir.fmt1.rd];
}

void VirtualMachine::op_add() {
  int32_t temp = r[ir.fmt0.rd];
  temp += r[ir.fmt0.rs];
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt0.rd] = temp & 0xffff;
}

void VirtualMachine::op_addi() {
  int32_t temp = r[ir.fmt1.rd];
  temp += ir.fmt1.constant;
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt1.rd] = temp & 0xffff;
}

void VirtualMachine::op_addc() {
  int32_t temp = r[ir.fmt0.rd];
  temp += r[ir.fmt0.rs];
  ++temp;
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt0.rd] = temp & 0xffff;
}

void VirtualMachine::op_addci() {
  int32_t temp = r[ir.fmt1.rd];
  temp += ir.fmt1.constant;
  ++temp;
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt1.rd] = temp & 0xffff;
}

void VirtualMachine::op_sub() {
  int32_t temp = r[ir.fmt0.rd];
  temp -= r[ir.fmt0.rs];
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt0.rd] = temp & 0xffff;
}

void VirtualMachine::op_subi() {
  int32_t temp = r[ir.fmt1.rd];
  temp -= ir.fmt1.constant;
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt0.rd] = temp & 0xffff;
}

void VirtualMachine::op_subc() {
  int32_t temp = r[ir.fmt0.rd];
  temp -= r[ir.fmt0.rs];
  --temp;
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt0.rd] = temp & 0xffff;
}

void VirtualMachine::op_subci() {
  int32_t temp = r[ir.fmt1.rd];
  temp -= ir.fmt1.constant;
  --temp;
  if (temp & 0x00010000) bts_carry();
  r[ir.fmt1.rd] = temp & 0xffff;
}

void VirtualMachine::op_and() {
  r[ir.fmt0.rd] &= r[ir.fmt0.rs];
}

void VirtualMachine::op_andi() {
  r[ir.fmt1.rd] &= ir.fmt1.constant;
}

void VirtualMachine::op_xor() {
  r[ir.fmt0.rd] ^= r[ir.fmt0.rs];
}

void VirtualMachine::op_xori() {
  r[ir.fmt0.rd] ^= ir.fmt1.constant;
}

void VirtualMachine::op_compl() {
  r[ir.fmt0.rd] = ~r[ir.fmt0.rd];
}

// Logical left shift; does not retain sign bit
void VirtualMachine::op_shl() {
  if (r[ir.fmt0.rd] & 0x80) bts_carry();
  r[ir.fmt0.rd] <<= 1;
}

// Arithmetic left shirt; retains sign bit
void VirtualMachine::op_shla() {
  if (r[ir.fmt0.rd] & 0x80) bts_carry();
  r[ir.fmt0.rd] = r[ir.fmt0.rd] & 0x80 | (r[ir.fmt0.rd] << 1) & 0x7f;
}

// Logical right bit; does not retain sign bit
void VirtualMachine::op_shr() {
  if (r[ir.fmt0.rd] & 0x01) bts_carry();
  r[ir.fmt0.rd] = (r[ir.fmt0.rd] >> 1) & 0x7f;
}

// Arithmetic right shift; retains sign bit
void VirtualMachine::op_shra() {
  if (r[ir.fmt0.rd] & 0x01) bts_carry();
  r[ir.fmt0.rd] >>= 1;
}

void VirtualMachine::op_compr() {
  if (r[ir.fmt0.rd] < r[ir.fmt0.rs])
    bts_less(), btr_equal(), btr_greater();
  else if (r[ir.fmt0.rd] == r[ir.fmt0.rs])
    btr_less(), bts_equal(), btr_greater();
  else
    btr_less(), btr_equal(), bts_greater();
}

void VirtualMachine::op_compri() {
  if (r[ir.fmt1.rd] < ir.fmt1.constant)
    bts_less(), btr_equal(), btr_greater();
  else if (r[ir.fmt1.rd] == ir.fmt1.constant)
    btr_less(), bts_equal(), btr_greater();
  else
    btr_less(), btr_equal(), bts_greater();
}

void VirtualMachine::op_getstat() {
  r[ir.fmt0.rd] = sr;
}

void VirtualMachine::op_putstat() {
  sr = r[ir.fmt0.rd];
}

void VirtualMachine::op_jump() {
  pc = ir.fmt1.addr;
}

void VirtualMachine::op_jumpl() {
  if (bt_less()) pc = ir.fmt1.addr;
}

void VirtualMachine::op_jumpe() {
  if (bt_equal()) pc = ir.fmt1.addr;
}

void VirtualMachine::op_jumpg() {
  if (bt_greater()) pc = ir.fmt1.addr;
}

void VirtualMachine::op_call() {
  if (sp < limit + 6) {
    fprintf(stderr, "Exceeded max stack depth\n");
    exit(1);
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

void VirtualMachine::op_return() {
  if (sp >= 256)
    exit(0);

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
  ++sp;
}

void VirtualMachine::op_read() {
  dot_in_file >> r[ir.fmt0.rd];
}

void VirtualMachine::op_write() {
  dot_out_file << r[ir.fmt0.rd] << std::endl;
}

void VirtualMachine::op_halt() {
  halt = true;
}

void VirtualMachine::op_noop() {

}
