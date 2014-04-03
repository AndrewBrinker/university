/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./VirtualMachine.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <iterator>

#define REG_FILE_SIZE 4
#define MEM_SIZE      256

static inline bool addition_overflow(uint16_t a, uint16_t b) {
  return (a + b < a);
}

static inline bool subtraction_overflow(uint16_t a, uint16_t b) {
  return (a - b > a);
}

VirtualMachine::VirtualMachine()
  : VirtualMachine(REG_FILE_SIZE, MEM_SIZE)
{}


VirtualMachine::VirtualMachine(uint16_t reg_file_size,
                               uint16_t mem_size) :
                              r(reg_file_size),
                              mem(mem_size),
                              pc(0),
                              sp(256),
                              base(0)
{}


void VirtualMachine::run(std::string inFilename) {
  // For op_read and op_write, we need to know the base filename.
  auto it_period = std::find(
    std::begin(inFilename),
    std::end(inFilename),
    '.');
  if (it_period == std::end(inFilename)) {
    std::cerr << "Sorry! Object file must end with \".o\"." << std::endl;
    return;
  }
  std::copy(
    std::begin(inFilename),
    it_period,
    std::back_inserter(base_filename));
  uint8_t op, rd, rs;
  union {
    uint8_t addr;
    int8_t c;
  } ac;
  bool i;

  std::basic_ifstream<uint16_t> inFile(inFilename, std::ios::binary);
  // Get size of file
  inFile.seekg(0, std::ios::end);
  limit = inFile.tellg();
  inFile.seekg(0, std::ios::beg);

  if (limit > 256) {
    std::cerr << "Program is too large to fit into memory." << std::endl;
    inFile.close();
    return;
  }

  // Load file into memory
  std::copy_n(
    std::istreambuf_iterator<uint16_t>(inFile),
    limit,
    std::begin(r));

  inFile.close();

  while (true) {
    ir = mem[pc];
    ++pc;

    op = ir >> 11 & 0x001F;
    rd = ir >> 9 & 0x0003;
    i = ir & 0x0100;
    rs = ir >> 6 & 0x0003;
    ac.addr = ir & 0x00FF;

    switch (op) {
    case 0u:
      if (i) {
        clock += 4;
        op_load(rd, ac.addr);
      } else {
        ++clock;
        op_loadi(rd, ac.c);
      }
      break;
    case 1u:
      clock += 4;
      op_store(rd, ac.addr);
      break;
    case 2u:
      ++clock;
      i ? op_add(rd, rs) : op_addi(rd, ac.c);
      break;
    case 3u:
      ++clock;
      i ? op_addc(rd, rs) : op_addci(rd, ac.c);
      break;
    case 4u:
      ++clock;
      i ? op_sub(rd, rs) : op_subi(rd, ac.c);
      break;
    case 5u:
      ++clock;
      i ? op_subc(rd, rs) : op_subci(rd, ac.c);
      break;
    case 6u:
      ++clock;
      i ? op_and(rd, rs) : op_andi(rd, ac.c);
      break;
    case 7u:
      ++clock;
      i ? op_xor(rd, rs) : op_xori(rd, ac.c);
      break;
    case 8u:
      ++clock;
      op_compl(rd);
      break;
    case 9u:
      ++clock;
      op_shl(rd);
      break;
    case 10u:
      ++clock;
      op_shla(rd);
      break;
    case 11u:
      ++clock;
      op_shr(rd);
      break;
    case 12u:
      ++clock;
      op_shra(rd);
      break;
    case 13u:
      ++clock;
      i ? op_compr(rd, rs) : op_compri(rd, ac.c);
      break;
    case 14u:
      ++clock;
      op_getstat(rd);
      break;
    case 15u:
      ++clock;
      op_putstat(rd);
      break;
    case 16u:
      ++clock;
      op_jump(ac.addr);
      break;
    case 17u:
      ++clock;
      op_jumpl(ac.addr);
      break;
    case 18u:
      ++clock;
      op_jumpe(ac.addr);
      break;
    case 19u:
      ++clock;
      op_jumpg(ac.addr);
      break;
    case 20u:
      clock += 4;
      op_call(ac.addr);
    case 21u:
      clock += 4;
      op_return();
      break;
    case 22u:
      clock += 28;
      op_read(rd);
      break;
    case 23u:
      clock += 28;
      op_write(rd);
      break;
    case 24:
      ++clock;
      return;
      break;
    case 25:
      ++clock;
      break;
    }
  }
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

void VirtualMachine::op_load(uint8_t rd, uint8_t addr) {
  r[rd] = mem[addr];
}

void VirtualMachine::op_loadi(uint8_t rd, uint8_t c) {
  r[rd] = c;
}

void VirtualMachine::op_store(uint8_t rd, uint8_t addr) {
  mem[addr] = r[rd];
}

void VirtualMachine::op_add(uint8_t rd, uint8_t rs) {
  uint8_t temp = r[rd];
  r[rd] += r[rs];
  if (addition_overflow(temp, r[rs]))
    bts_carry();
}

void VirtualMachine::op_addi(uint8_t rd, uint8_t c) {
  uint8_t temp = r[rd];
  r[rd] += c;
  if (addition_overflow(temp, c))
    bts_carry();
}

void VirtualMachine::op_addc(uint8_t rd, uint8_t rs) {
  uint8_t temp = r[rd];
  r[rd] += r[rs] + (bt_carry() ? 1 : 0);
  if (addition_overflow(temp, r[rs] + (bt_carry() ? 1 : 0)))
    bts_carry();
}

void VirtualMachine::op_addci(uint8_t rd, uint8_t c) {
  uint8_t temp = r[rd];
  r[rd] += c + (bt_carry() ? 1 : 0);
  if (addition_overflow(temp, c + (bt_carry() ? 1 : 0)))
    bts_carry();
}

void VirtualMachine::op_sub(uint8_t rd, uint8_t rs) {
  uint8_t temp = r[rd];
  r[rd] -= r[rs];
  if (subtraction_overflow(temp, r[rs]))
    bts_carry();
}

void VirtualMachine::op_subi(uint8_t rd, uint8_t c) {
  uint8_t temp = r[rd];
  r[rd] -= c;
  if (subtraction_overflow(temp, c))
    bts_carry();
}

void VirtualMachine::op_subc(uint8_t rd, uint8_t rs) {
  uint8_t temp = r[rd];
  r[rd] -= r[rs] + (bt_carry() ? 1 : 0);
  if (subtraction_overflow(temp, r[rs] + (bt_carry() ? 1 : 0)))
    bts_carry();
}

void VirtualMachine::op_subci(uint8_t rd, uint8_t c) {
  uint8_t temp = r[rd];
  r[rd] -= c + (bt_carry() ? 1 : 0);
  if (subtraction_overflow(temp, c + (bt_carry() ? 1 : 0)))
    bts_carry();
}

void VirtualMachine::op_and(uint8_t rd, uint8_t rs) {
  r[rd] &= r[rs];
}

void VirtualMachine::op_andi(uint8_t rd, uint8_t c) {
  r[rd] &= c;
}

void VirtualMachine::op_xor(uint8_t rd, uint8_t rs) {
  r[rd] ^= r[rs];
}

void VirtualMachine::op_xori(uint8_t rd, uint8_t c) {
  r[rd] ^= c;
}

void VirtualMachine::op_compl(uint8_t rd) {
  r[rd] = ~r[rd];
}

void VirtualMachine::op_shl(uint8_t rd) {}

void VirtualMachine::op_shla(uint8_t rd) {}

void VirtualMachine::op_shr(uint8_t rd) {}

void VirtualMachine::op_shra(uint8_t rd) {}

void VirtualMachine::op_compr(uint8_t rd, uint8_t rs) {
  if (r[rd] < r[rs])
    bts_less(), btr_equal(), btr_greater();
  else if (r[rd] == r[rs])
    btr_less(), bts_equal(), btr_greater();
  else
    btr_less(), btr_equal(), bts_greater();
}

void VirtualMachine::op_compri(uint8_t rd, uint8_t c) {
  if (r[rd] < c)
    bts_less(), btr_equal(), btr_greater();
  else if (r[rd] == c)
    btr_less(), bts_equal(), btr_greater();
  else
    btr_less(), btr_equal(), bts_greater();
}

void VirtualMachine::op_getstat(uint8_t rd) {
  r[rd] = sr;
}

void VirtualMachine::op_putstat(uint8_t rd) {
  sr = r[rd];
}

void VirtualMachine::op_jump(uint8_t addr) {
  pc = addr;
}

void VirtualMachine::op_jumpl(uint8_t addr) {
  if (bt_less()) pc = addr;
}

void VirtualMachine::op_jumpe(uint8_t addr) {
  if (bt_equal()) pc = addr;
}

void VirtualMachine::op_jumpg(uint8_t addr) {
  if (bt_greater()) pc = addr;
}

void VirtualMachine::op_call(uint8_t addr) {}

void VirtualMachine::op_return() {}

void VirtualMachine::op_read(uint8_t rd) {
  std::ifstream inFile(base_filename + ".in");
  inFile >> r[rd];
  inFile.close();
}

void VirtualMachine::op_write(uint8_t rd) {
  std::ofstream outFile(base_filename + ".out");
  outFile << r[rd];
  outFile.close();
}
