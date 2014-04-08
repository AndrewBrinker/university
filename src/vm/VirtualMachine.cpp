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
                              base(0) {
  Opcode_t value;
  for (int i = 0; i < 256; ++i) {
    value.i = i << 8;
    switch (value.fmt0.op) {
    case 0:
      if (value.fmt0.i == 0) {
        clocks[i] = 4;
        ops[i] = [&] () {  // load
          r[ir.fmt1.rd] = mem[ir.fmt1.addr];
        };
      } else {
        clocks[i] = 1;
        ops[i] = [&] () {  // loadi
          r[ir.fmt1.rd] = ir.fmt1.constant;
        };
      }
      break;
    case 1:
      clocks[i] = 4;
      ops[i] = [&] () {  // store
        mem[ir.fmt1.addr] = r[ir.fmt1.rd];
      };
      break;
    case 2:
      clocks[i] = 1;
      if (value.fmt0.i == 0) {
        ops[i] = [&] () {  // add
          int32_t temp = r[ir.fmt0.rd];
          temp += r[ir.fmt0.rs];
          if (temp & 0x00010000) bts_carry();
          r[ir.fmt0.rd] = temp & 0xffff;
        };
      } else {
        ops[i] = [&] () {  // addi
          int32_t temp = r[ir.fmt1.rd];
          temp += ir.fmt1.constant;
          if (temp & 0x00010000) bts_carry();
          r[ir.fmt1.rd] = temp & 0xffff;
        };
      }
      break;
    case 3:
      clocks[i] = 1;
      if (value.fmt0.i == 0) {
        ops[i] = [&] () {  // addc
          int32_t temp = r[ir.fmt0.rd];
          temp += r[ir.fmt0.rs];
          ++temp;
          if (temp & 0x00010000) bts_carry();
          r[ir.fmt0.rd] = temp & 0xffff;
        };
      } else {
        ops[i] = [&] () {  // addci
          int32_t temp = r[ir.fmt1.rd];
          temp += ir.fmt1.constant;
          ++temp;
          if (temp & 0x00010000) bts_carry();
          r[ir.fmt1.rd] = temp & 0xffff;
        };
      }
      break;
    case 4:
      clocks[i] = 1;
      if (value.fmt0.i == 0) {
        ops[i] = [&] () {  // sub
          int32_t temp = r[ir.fmt0.rd];
          temp -= r[ir.fmt0.rs];
          if (temp & 0x00010000) bts_carry();
          r[ir.fmt0.rd] = temp & 0xffff;
        };
      } else {
        ops[i] = [&] () {  // subi
          int32_t temp = r[ir.fmt1.rd];
          temp -= ir.fmt1.constant;
          if (temp & 0x00010000) bts_carry();
          r[ir.fmt0.rd] = temp & 0xffff;
        };
      }
      break;
    case 5:
      clocks[i] = 1;
      if (value.fmt0.i == 0) {
        ops[i] = [&] () {  // subc
          int32_t temp = r[ir.fmt0.rd];
          temp -= r[ir.fmt0.rs];
          --temp;
          if (temp & 0x00010000) bts_carry();
          r[ir.fmt0.rd] = temp & 0xffff;
        };
      } else {
        ops[i] = [&] () {  // subci
          int32_t temp = r[ir.fmt1.rd];
          temp -= ir.fmt1.constant;
          --temp;
          if (temp & 0x00010000) bts_carry();
          r[ir.fmt1.rd] = temp & 0xffff;
        };
      }
      break;
    case 6:
      clocks[i] = 1;
      if (value.fmt0.i == 0) {
        ops[i] = [&] () {  // and
          r[ir.fmt0.rd] &= r[ir.fmt0.rs];
        };
      } else {
        ops[i] = [&] () {  // andi
          r[ir.fmt1.rd] &= ir.fmt1.constant;
        };
      }
      break;
    case 7:
      clocks[i] = 1;
      if (value.fmt0.i == 0) {
        ops[i] = [&] () {  // xor
          r[ir.fmt0.rd] ^= r[ir.fmt0.rs];
        };
      } else {
        ops[i] = [&] () {  // xori
          r[ir.fmt1.rd] ^= ir.fmt1.constant;
        };
      }
      break;
    case 8:
      clocks[i] = 1;
      ops[i] = [&] () {  // compl
        r[ir.fmt0.rd] = ~r[ir.fmt0.rd];
      };
      break;
    case 9:
      clocks[i] = 1;
      ops[i] = [&] () {  // shl
        if (r[ir.fmt0.rd] & 0x80) bts_carry();
        r[ir.fmt0.rd] <<= 1;
      };
      break;
    case 10:
      clocks[i] = 1;
      ops[i] = [&] () {  // shla
        if (r[ir.fmt0.rd] & 0x80) bts_carry();
        r[ir.fmt0.rd] <<= 1;
      };
      break;
    case 11:
      clocks[i] = 1;
      ops[i] = [&] () {  // shr
        if (r[ir.fmt0.rd] & 0x01) bts_carry();
        r[ir.fmt0.rd] >>= 1;
      };
      break;
    case 12:
      clocks[i] = 1;
      ops[i] = [&] () {  // shra
        if (r[ir.fmt0.rd] & 0x01) bts_carry();
        r[ir.fmt0.rd] >>= 1;
      };
      break;
    case 13:
      clocks[i] = 1;
      if (i == 0) {
        ops[i] = [&] () {  // compr
          if (r[ir.fmt0.rd] < r[ir.fmt0.rs]) {
            bts_less(); btr_equal(); btr_greater();
          } else if (r[ir.fmt0.rd] == r[ir.fmt0.rs]) {
            btr_less(); bts_equal(); btr_greater();
          } else {
            btr_less(); btr_equal(); bts_greater();
          }
        };
      } else {
        ops[i] = [&] () {  // compri
          if (r[ir.fmt1.rd] < ir.fmt1.constant) {
            bts_less(); btr_equal(); btr_greater();
          } else if (r[ir.fmt1.rd] == ir.fmt1.constant) {
            btr_less(); bts_equal(); btr_greater();
          } else {
            btr_less(); btr_equal(); bts_greater();
          }
        };
      }
      break;
    case 14:
      clocks[i] = 1;
      ops[i] = [&] () {  // getstat
        r[ir.fmt0.rd] = sr;
      };
      break;
    case 15:
      clocks[i] = 1;
      ops[i] = [&] () {  // putstat
        sr = r[ir.fmt0.rd];
      };
      break;
    case 16:
      clocks[i] = 1;
      ops[i] = [&] () {  // jump
        pc = ir.fmt1.addr;
      };
      break;
    case 17:
      clocks[i] = 1;
      ops[i] = [&] () {  // jumpl
        if (bt_less()) pc = ir.fmt1.addr;
      };
      break;
    case 18:
      clocks[i] = 1;
      ops[i] = [&] () {  // jumpe
        if (bt_equal()) pc = ir.fmt1.addr;
      };
      break;
    case 19:
      clocks[i] = 1;
      ops[i] = [&] () {  // jumpg
        if (bt_greater()) pc = ir.fmt1.addr;
      };
      break;
    case 20:
      clocks[i] = 4;
      ops[i] = [&] () {  // call
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
      };
      break;
    case 21:
      clocks[i] = 4;
      ops[i] = [&] () {  // return
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
      };
      break;
    case 22:
      clocks[i] = 28;
      ops[i] = [&] () {  // read
        if (!(dot_in_file.is_open())) {
          fprintf(stderr, "Could not open .in file\n");
          exit(1);
        }
        if (!(dot_in_file >> r[ir.fmt0.rd])) {
          fprintf(stderr, "Could not read from .in file\n");
          exit(1);
        }
      };
      break;
    case 23:
      clocks[i] = 28;
      ops[i] = [&] () {  // write
        dot_out_file << r[ir.fmt0.rd] << std::endl;
      };
      break;
    case 24:
      clocks[i] = 1;
      ops[i] = [&] () {  // halt
        exit(0);
      };
      break;
    case 25:
      clocks[i] = 1;
      ops[i] = [&] () {};  // noop
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

VirtualMachine::~VirtualMachine() {
  dot_out_file << clock << std::endl;
  dot_in_file.close();
  dot_out_file.close();
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
/*
  uint8_t op, rd, rs;
  union {
    uint8_t addr;
    int8_t c;
  } ac;
  bool i;
*/
  std::ifstream inFile(inFilename, std::ios::binary);
  // Get size of file
  inFile.seekg(0, std::ios::end);
  limit = inFile.tellg();
  inFile.seekg(0, std::ios::beg);

  // because size will be in bytes, not in 16-bit words
  if (limit > 256 * 2) {
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

  // main loop
  while (true) {
    ir.i = mem[pc];
    ++pc;
    ops[ir.i >> 8]();
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
/*
void VirtualMachine::op_load(uint8_t rd, uint8_t addr) {
  r[rd] = mem[addr];
}

void VirtualMachine::op_loadi(uint8_t rd, int8_t c) {
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

void VirtualMachine::op_addi(uint8_t rd, int8_t c) {
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

void VirtualMachine::op_addci(uint8_t rd, int8_t c) {
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

void VirtualMachine::op_subi(uint8_t rd, int8_t c) {
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

void VirtualMachine::op_subci(uint8_t rd, int8_t c) {
  uint8_t temp = r[rd];
  r[rd] -= c + (bt_carry() ? 1 : 0);
  if (subtraction_overflow(temp, c + (bt_carry() ? 1 : 0)))
    bts_carry();
}

void VirtualMachine::op_and(uint8_t rd, uint8_t rs) {
  r[rd] &= r[rs];
}

void VirtualMachine::op_andi(uint8_t rd, int8_t c) {
  r[rd] &= c;
}

void VirtualMachine::op_xor(uint8_t rd, uint8_t rs) {
  r[rd] ^= r[rs];
}

void VirtualMachine::op_xori(uint8_t rd, int8_t c) {
  r[rd] ^= c;
}

void VirtualMachine::op_compl(uint8_t rd) {
  r[rd] = ~r[rd];
}

void VirtualMachine::op_shl(uint8_t rd) {
  if (r[rd] & 0x0080)
    bts_carry();
  r[rd] <<= 1;
}

void VirtualMachine::op_shla(uint8_t rd) {
  if (r[rd] & 0x0080)
    bts_carry();
  r[rd] <<= 1;
}

void VirtualMachine::op_shr(uint8_t) {}

void VirtualMachine::op_shra(uint8_t) {}

void VirtualMachine::op_compr(uint8_t rd, uint8_t rs) {
  if (r[rd] < r[rs])
    bts_less(), btr_equal(), btr_greater();
  else if (r[rd] == r[rs])
    btr_less(), bts_equal(), btr_greater();
  else
    btr_less(), btr_equal(), bts_greater();
}

void VirtualMachine::op_compri(uint8_t rd, int8_t c) {
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

void VirtualMachine::op_call(uint8_t addr) {
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

  pc = addr;
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

void VirtualMachine::op_read(uint8_t rd) {
  dot_in_file >> r[rd];
}

void VirtualMachine::op_write(uint8_t rd) {
  dot_out_file << r[rd] << std::endl;
}
*/
