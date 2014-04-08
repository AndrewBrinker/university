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

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class VirtualMachine {
 public:
  VirtualMachine();
  VirtualMachine(uint16_t, uint16_t);

  void run(std::string);

 private:
  std::vector<int16_t> r;
  std::vector<int16_t> mem;

  uint16_t pc, sr, sp, base, limit, clock;
  bool halt;

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

  std::function<void(void)> ops[256];
  uint8_t clocks[256];

  std::string base_filename;
  std::ifstream dot_in_file;
  std::ofstream dot_out_file;

  bool bt_overflow() const;
  bool btr_overflow();
  bool bts_overflow();

  bool bt_less() const;
  bool btr_less();
  bool bts_less();

  bool bt_equal() const;
  bool btr_equal();
  bool bts_equal();

  bool bt_greater() const;
  bool btr_greater();
  bool bts_greater();

  bool bt_carry() const;
  bool btr_carry();
  bool bts_carry();

  void op_load(uint8_t, uint8_t);
  void op_loadi(uint8_t, int8_t);
  void op_store(uint8_t, uint8_t);
  void op_add(uint8_t, uint8_t);
  void op_addi(uint8_t, int8_t);
  void op_addc(uint8_t, uint8_t);
  void op_addci(uint8_t, int8_t);
  void op_sub(uint8_t, uint8_t);
  void op_subi(uint8_t, int8_t);
  void op_subc(uint8_t, uint8_t);
  void op_subci(uint8_t, int8_t);
  void op_and(uint8_t, uint8_t);
  void op_andi(uint8_t, int8_t);
  void op_xor(uint8_t, uint8_t);
  void op_xori(uint8_t, int8_t);
  void op_compl(uint8_t);
  void op_shl(uint8_t);
  void op_shla(uint8_t);
  void op_shr(uint8_t);
  void op_shra(uint8_t);
  void op_compr(uint8_t, uint8_t);
  void op_compri(uint8_t, int8_t);
  void op_getstat(uint8_t);
  void op_putstat(uint8_t);
  void op_jump(uint8_t);
  void op_jumpl(uint8_t);
  void op_jumpe(uint8_t);
  void op_jumpg(uint8_t);
  void op_call(uint8_t);
  void op_return();
  void op_read(uint8_t);
  void op_write(uint8_t);

  DISALLOW_COPY_AND_ASSIGN(VirtualMachine);
};

#endif  // VIRTUAL_MACHINE_H
