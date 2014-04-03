/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./VirtualMachine.h"
#include <cstdint>

#define REG_FILE_SIZE 4
#define MEM_SIZE      256

static inline bool addition_overflow(uint16_t a, uint16_t b) {
  return (a + b < a);
}

static inline bool subtraction_overflow(uint16_t a, uint16_t b) {
  return (a - b > a);
}

VirtualMachine::VirtualMachine()
  : VirtualMachine(REG_FILE_SIZE, MEM_SIZE) {}


VirtualMachine::VirtualMachine(uint16_t,
                               uint16_t mem_size) :
                               mem(mem_size) {}


void VirtualMachine::run(Assembler::ObjectSource) {
    return;
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

void VirtualMachine::op_read(uint8_t rd) {}

void VirtualMachine::op_write(uint8_t rd) {}

void VirtualMachine::op_halt() {}

void VirtualMachine::op_noop() {}
