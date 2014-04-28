/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./OS.h"
#include <dir/Dir.h>
#include <util/Utilities.h>
#include <string>
#include <vector>
#include <cstdio>
#include <memory>   // std::unique_ptr
#include <utility>  // std::move

#define N_REGISTERS 4

#define TIME_SLICE_TIME 15

#define RETURN_STATUS_MASK 07
#define RETURN_STATUS_SHIFT 5
#define IO_REGISTER_MASK 03
#define IO_REGISTER_SHIFT 8

#define IO_SYSTEM_TIME 27

OS::OS()
  : as(new Assembler),
    vm(new VirtualMachine),

    system_time(0),
    system_utilization(0.0),
    user_utilization(0.0),
    vm_throughput(0.0),
    idle_time(0)
{}

void OS::run() {
  // Find all *.s files and load their names (and paths) into memory
  std::vector<std::string> source_files = get_sourcefiles();

  // Assemble each file, load it into memory, create its PCB, and put into
  // the ready queue
  std::string object_file;
  std::unique_ptr<PCB> curr_pcb;

  for (std::string source_file : source_files) {
    curr_pcb = std::unique_ptr<PCB>(new PCB(source_file));
#ifdef DEBUG
    printf("Attempting to parse %s... ", source_file.c_str());
#endif  // DEBUG
    as->parse(curr_pcb.get());
#ifdef DEBUG
    puts("Parsed successfully!");
    printf("Attempting to load %s... ", source_file.c_str());
#endif  // DEBUG
    vm->load(curr_pcb.get());
#ifdef DEBUG
    puts("Loaded successfully!\n");
#endif  // DEBUG
    ready.push(std::move(curr_pcb));
  }

#ifdef DEBUG
  printf("Contents of ready queue:\n\t");
  PCB* front = ready.front().get();
  do {
    printf("%s ", ready.front()->process_name.c_str());
    ready.push(std::move(ready.front()));
    ready.pop();
  } while (front != ready.front().get());
  puts("");
#endif  // DEBUG

  // Main loop
  while (!ready.empty() && !waiting.empty()) {
    // If IO is complete
    if (waiting.front()->interrupt_time >= system_time) {
      ready.push(std::move(waiting.front()));
      waiting.pop();
    }

    if (!ready.empty()) {
      run_next_process();
    } else {
      // process waiting processes
      idle_time += waiting.front()->interrupt_time - system_time;
      system_time = waiting.front()->interrupt_time;
    }
  }
}

// Load the next process into the Virtual Machine and run it for its time slice.
void OS::run_next_process() {
  running = std::move(ready.front());
  ready.pop();
  system_time += vm->run_process(running.get(), TIME_SLICE_TIME);

  ReturnStatus_t return_status =
    static_cast<ReturnStatus_t>(getReturnStatus(running.get()));
  uint8_t io_register;
  switch (return_status) {
  case ReturnStatus_t::TIME_SLICE :
    ready.push(std::move(running));
    break;
  case ReturnStatus_t::HALT_INSTRUCTION :
    running.reset();
    break;
  case ReturnStatus_t::OUT_OF_BOUND_REFERENCE :
    fprintf(stderr, "Out of bound reference in process %s\n",
            running->process_name.c_str());
    running.reset();
    break;
  case ReturnStatus_t::STACK_OVERFLOW :
    fprintf(stderr, "Stack overflow in process %s\n",
            running->process_name.c_str());
    running.reset();
    break;
  case ReturnStatus_t::STACK_UNDERFLOW :
    fprintf(stderr, "Stack underflow in process %s\n",
            running->process_name.c_str());
    running.reset();
    break;
  case ReturnStatus_t::INVALID_OPCODE :
    fprintf(stderr, "Invalid opcode in process %s\n",
            running->process_name.c_str());
    running.reset();
    break;
  case ReturnStatus_t::READ_OPERATION :
    io_register = getIORegister(running.get());
    running->in_file >> running->r[io_register];
    running->interrupt_time = system_time + IO_SYSTEM_TIME;
    waiting.push(std::move(running));
    break;
  case ReturnStatus_t::WRITE_OPERATION :
    io_register = getIORegister(running.get());
    running->out_file << running->r[io_register] << std::endl;
    running->interrupt_time = system_time + IO_SYSTEM_TIME;
    waiting.push(std::move(running));
    break;
  }
}

inline uint8_t OS::getReturnStatus(const PCB* pcb) const {
  return (pcb->sr &
          (RETURN_STATUS_MASK << RETURN_STATUS_SHIFT) >> RETURN_STATUS_SHIFT);
}

inline uint8_t OS::getIORegister(const PCB* pcb) const {
  return (pcb->sr &
          (IO_REGISTER_MASK << IO_REGISTER_SHIFT) >> IO_REGISTER_SHIFT);
}

// Find all *.s files and load their names (and paths) into memory
std::vector<std::string> OS::get_sourcefiles() {
  std::vector<std::string> filenames;
  for (auto de : walk()) {
    for (std::string filename : de.files) {
      if (has_suffix(filename, ".s")) {
        filenames.push_back(filename);
      }
    }
  }
  return filenames;
}
