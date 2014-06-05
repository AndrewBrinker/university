/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef OS_H
#define OS_H

#include <asm/Assembler.h>
#include <vm/VirtualMachine.h>
#include <pcb/PCB.h>
#include <cstdint>
#include <list>
#include <queue>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete;
#endif

#define N_REGISTERS 4

class OS {
 public:
  OS();
  void run(std::string);

 private:
  uint8_t getReturnStatus(const PCB* pcb) const;
  uint8_t getIORegister(const PCB* pcb) const;
  std::vector<std::string> getSourceFiles();
  void assignNextProcess();
  void contextSwitch();

  std::unique_ptr<Assembler> as;
  std::unique_ptr<VirtualMachine> vm;
  std::list<std::unique_ptr<PCB>> jobs;
  std::queue<const PCB*> ready;
  std::queue<const PCB*> waiting;
  PCB* running;
  uint32_t system_time;
  uint32_t idle_time;

  DISALLOW_COPY_AND_ASSIGN(OS)
};

#endif  // OS_H
