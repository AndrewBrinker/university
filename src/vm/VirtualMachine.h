/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <pcb/PCB.h>
#include <tlb/TLB.h>
#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <functional>

#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete;
#endif

enum ReturnStatus_t {
  TIME_SLICE             = 00,
  HALT_INSTRUCTION       = 01,
  OUT_OF_BOUND_REFERENCE = 02,
  STACK_OVERFLOW         = 03,
  STACK_UNDERFLOW        = 04,
  INVALID_OPCODE         = 05,
  READ_OPERATION         = 06,
  WRITE_OPERATION        = 07
};

class VirtualMachine {
 public:
  VirtualMachine();
  void loadIntoMemory(PCB* pcb);
  uint8_t runProcess(PCB* pcb, uint8_t time_slice);

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

  TLB tlb;
  std::array<uint16_t, N_FRAMES> frame_registers;

  std::vector<int16_t> r;
  std::vector<int16_t> mem;
  uint16_t pc, sr, sp, base, limit;
  bool halt;
  void (VirtualMachine::*ops[256])();
  uint8_t clocks[256];

  void loadFile(std::fstream& object_file);
  void loadPCB(PCB* pcb);
  void unloadPCB(PCB* pcb);

  void readStack(std::fstream& stack_file);
  void writeStack(std::fstream& stack_file);

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

  uint8_t getReturnStatus() const;
  uint8_t setReturnStatus(uint8_t status);

  uint8_t getIO_Register() const;
  uint8_t setIO_Register(uint8_t reg);

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

  DISALLOW_COPY_AND_ASSIGN(VirtualMachine)
};

#endif  // VIRTUAL_MACHINE_H
