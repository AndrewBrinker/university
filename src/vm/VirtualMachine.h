/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <functional>

#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;               \
  void operator=(const TypeName&) = delete;
#endif

class OS;

class VirtualMachine {
 public:
  VirtualMachine();
  void run(std::string);

  void setMemory(int);  // NOLINT

 private:
  union Opcode_t {
    uint16_t i;
    struct {
      uint8_t unused  : 6;
      uint8_t rs      : 2;
      uint8_t i       : 1;
      uint8_t rd      : 2;
      uint8_t op      : 5;
    } fmt0;
    struct {
      union {
        uint8_t addr;
        int8_t constant;
      };
      uint8_t i       : 1;
      uint8_t rd      : 2;
      uint8_t op      : 5;
    } fmt1;
  } ir;
  typedef void (VirtualMachine::*MemFuncPtr)();

  std::vector<int16_t> r;
  std::vector<int16_t> mem;
  uint16_t pc, sr, sp, base, limit, clock;
  bool halt;
  MemFuncPtr ops[256];
  uint8_t clocks[256];
  std::string base_file_name;
  std::ifstream dot_in_file;
  std::ofstream dot_out_file;

  bool getOverflow() const;
  bool setOverflow(bool b);

  bool getLess() const;
  bool setLess(bool b);

  bool getEqual() const;
  bool setEqual(bool b);

  bool getGreater() const;
  bool setGreater(bool b);

  bool getCarry() const;
  bool setCarry(bool b);

  void op(const Opcode_t&);

  void op_load();
  void op_loadi();
  void op_store();
  void op_add();
  void op_addi();
  void op_addc();
  void op_addci();
  void op_sub();
  void op_subi();
  void op_subc();
  void op_subci();
  void op_and();
  void op_andi();
  void op_xor();
  void op_xori();
  void op_compl();
  void op_shl();
  void op_shla();
  void op_shr();
  void op_shra();
  void op_compr();
  void op_compri();
  void op_getstat();
  void op_putstat();
  void op_jump();
  void op_jumpl();
  void op_jumpe();
  void op_jumpg();
  void op_call();
  void op_return();
  void op_read();
  void op_write();
  void op_halt();
  void op_noop();

  void setupOpMap();

  friend class OS;

  DISALLOW_COPY_AND_ASSIGN(VirtualMachine)
};

#endif  // VIRTUAL_MACHINE_H
