/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef PCB_H
#define PCB_H

#include <tbl/Table.h>
#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete;
#endif

#define REG_FILE_SIZE 4
#define MEM_SIZE      256

#define SOURCE_EXT  ".s"
#define OBJECT_EXT  ".o"
#define INPUT_EXT   ".in"
#define OUTPUT_EXT  ".out"
#define STACK_EXT   ".st"

#ifdef DEBUG
#define LOG_EXT     ".log"
#endif  // DEBUG

struct PCB {
  explicit PCB(std::string source_file);

  // Virtual Machine data
  uint16_t pc, sr, sp, base, limit;
  std::vector<int16_t> r;

  // Process name
  std::string name;

  // Files
  std::fstream s_file;
  std::fstream o_file;
  std::fstream in_file;
  std::fstream out_file;
  std::fstream st_file;

  // Accounting
  uint32_t vm_time;
  uint32_t waiting_time;
  uint32_t turnaround_time;
  uint32_t io_time;
  uint32_t largest_stack;
  uint32_t page_fault_count;
  uint32_t memory_access_count;
  uint32_t memory_hit_count;  // hit ratio = # memory hits / # memory accesses

  // Interrupt
  uint32_t interrupt_time;

  // Helpers
  uint32_t time_entered_ready_queue;
  uint32_t time_entered_waiting_queue;

  // TLB
  TLB tlb;

 private:
  DISALLOW_COPY_AND_ASSIGN(PCB)
};

#endif  // PCB_H
