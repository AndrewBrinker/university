/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./PCB.h"
#include <util/Utilities.h>
#include <string>

PCB::PCB(std::string source_file)
  : pc(0),
    sr(0),
    sp(MEM_SIZE - 1),
    base(0),
    limit(0),
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
    vm_time(0),
    waiting_time(0),
    turnaround_time(0),
    io_time(0),
    largest_stack_size(0),
    page_fault_count(0),
    hit_ratio(0),
    interrupt_time(0),
    time_entered_ready_queue(0),
    time_entered_waiting_queue(0) {}
