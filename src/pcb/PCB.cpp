/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./PCB.h"

#include <util/Utilities.h>
#include <string>

PCB::PCB(std::string source_file)
  : pc(0), sr(0), sp(MEM_SIZE - 1), base(0), limit(0),
    r(REG_FILE_SIZE, 0),

    process_name(stripPath(stripExtension(source_file))),

    s_file(stripExtension(source_file) + SOURCE_EXT),
    o_file(stripExtension(source_file) + OBJECT_EXT,
           std::ios::in | std::ios::out | std::ios::trunc),
    in_file(stripExtension(source_file) + INPUT_EXT),
    out_file(stripExtension(source_file) + OUTPUT_EXT,
           std::ios::out | std::ios::trunc),
    st_file(stripExtension(source_file) + STACK_EXT,
           std::ios::in | std::ios::out | std::ios::trunc),

    vm_time(0), waiting_time(0), turnaround_time(0), io_time(0),
    largest_stack_size(0),

    interrupt_time(0)
#ifdef DEBUG
    , log_file(stripExtension(source_file) + LOG_EXT,
               std::ios::out | std::ios::trunc)
#endif  // DEBUG
{
#ifdef DEBUG
  if (!s_file.is_open())
    printf("%s isn't open.\n", (process_name + SOURCE_EXT).c_str());
  if (!o_file.is_open())
    printf("%s isn't open.\n", (process_name + OBJECT_EXT).c_str());
  if (!in_file.is_open())
    printf("%s isn't open.\n", (process_name + INPUT_EXT).c_str());
  if (!out_file.is_open())
    printf("%s isn't open.\n", (process_name + OUTPUT_EXT).c_str());
  if (!st_file.is_open())
    printf("%s isn't open.\n", (process_name + STACK_EXT).c_str());
  if (!log_file.is_open())
    printf("%s isn't open.\n", (process_name + LOG_EXT).c_str());
#endif  // DEBUG
}
