/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef PCB_H
#define PCB_H

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
  std::string process_name;

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
  uint32_t largest_stack_size;

  // Interrupt
  uint32_t interrupt_time;
#ifdef DEBUG
  std::vector<std::string> asm_source;
  std::ofstream log_file;
#endif  // DEBUG

 private:
  DISALLOW_COPY_AND_ASSIGN(PCB)
};

#endif  // PCB_H
