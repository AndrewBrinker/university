/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */
#ifndef SRC_VM_VIRTUALMACHINE_H_
#define SRC_VM_VIRTUALMACHINE_H_

#include <vector>
#include <cstdint>

#include "../asm/Assembler.h"

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class VirtualMachine {
 public:
  VirtualMachine();
  VirtualMachine(uint16_t, uint16_t);
  void run(Assembler::ObjectSource);

 private:
  std::vector<int> r;
  std::vector<int> mem;

  uint16_t pc, ir, sr, sp, base, limit, clock;

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
  void op_loadi(uint8_t, uint8_t);
  void op_store(uint8_t, uint8_t);
  void op_add(uint8_t, uint8_t);
  void op_addi(uint8_t, uint8_t);
  void op_addc(uint8_t, uint8_t);
  void op_addci(uint8_t, uint8_t);
  void op_sub(uint8_t, uint8_t);
  void op_subi(uint8_t, uint8_t);
  void op_subc(uint8_t, uint8_t);
  void op_subci(uint8_t, uint8_t);
  void op_and(uint8_t, uint8_t);
  void op_andi(uint8_t, uint8_t);
  void op_xor(uint8_t, uint8_t);
  void op_xori(uint8_t, uint8_t);
  void op_compl(uint8_t);
  void op_shl(uint8_t);
  void op_shla(uint8_t);
  void op_shr(uint8_t);
  void op_shra(uint8_t);
  void op_compr(uint8_t, uint8_t);
  void op_compri(uint8_t, uint8_t);
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
  void op_halt();
  void op_noop();

  DISALLOW_COPY_AND_ASSIGN(VirtualMachine);
};

#endif  // SRC_VM_VIRTUALMACHINE_H_
