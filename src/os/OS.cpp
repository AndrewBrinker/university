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

/**
 * Initialize the internal variables.
 */
OS::OS()
  : as(new Assembler),
    vm(new VirtualMachine),
    system_time(0),
    system_utilization(0.0),
    user_utilization(0.0),
    vm_throughput(0.0),
    idle_time(0) {}


/**
 * Load all the assembly files, create the PCBs, and run them all with
 * time-sharing.
 */
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
    vm->load_into_memory(curr_pcb.get());
#ifdef DEBUG
    printf("Base of process %s is %d\n",
           curr_pcb->process_name.c_str(),
           curr_pcb->base);
    puts("Loaded successfully!\n");
#endif  // DEBUG
    ready.push(curr_pcb.get());
    jobs.push_back(std::move(curr_pcb));
  }

#ifdef DEBUG
  printf("Contents of jobs list:\n\t");
  for (const std::unique_ptr<PCB>& pcb : jobs)
    printf("%s ", pcb->process_name.c_str());
  puts("\n");

  printf("Contents of ready queue:\n\t");
  const PCB* front = ready.front();
  do {
    printf("%s ", ready.front()->process_name.c_str());
    ready.push(std::move(ready.front()));
    ready.pop();
  } while (front != ready.front());
  puts("");
#endif  // DEBUG

  // Main loop
  while (!ready.empty() || !waiting.empty()) {
    // If IO is complete
    if (!waiting.empty()) {
      if (waiting.front()->interrupt_time <= system_time) {
        ready.push(waiting.front());
        waiting.pop();
      }
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


/**
 * Load the next process into the Virtual Machine and run it for its time slice.
 */
void OS::run_next_process() {
  running = const_cast<PCB*>(ready.front());
  ready.pop();
#ifdef DEBUG
  printf("Beginning time slice for process %s\n",
         running->process_name.c_str());
#endif  // DEBUG
  system_time += vm->run_process(running, TIME_SLICE_TIME);

  ReturnStatus_t return_status =
    static_cast<ReturnStatus_t>(getReturnStatus(running));
  uint8_t io_register;
  switch (return_status) {
  case ReturnStatus_t::TIME_SLICE :
#ifdef DEBUG
    printf("Time slice completed for process %s\n",
           running->process_name.c_str());
#endif  // DEBUG
    ready.push(std::move(running));
    break;
  case ReturnStatus_t::HALT_INSTRUCTION :
#ifdef DEBUG
    printf("Process %s encountered halt instruction\n",
           running->process_name.c_str());
#endif  // DEBUG
    running = nullptr;
    break;
  case ReturnStatus_t::OUT_OF_BOUND_REFERENCE :
    fprintf(stderr, "Out of bound reference in process %s\n",
            running->process_name.c_str());
    running = nullptr;
    break;
  case ReturnStatus_t::STACK_OVERFLOW :
    fprintf(stderr, "Stack overflow in process %s\n",
            running->process_name.c_str());
    running = nullptr;
    break;
  case ReturnStatus_t::STACK_UNDERFLOW :
    fprintf(stderr, "Stack underflow in process %s\n",
            running->process_name.c_str());
    running = nullptr;
    break;
  case ReturnStatus_t::INVALID_OPCODE :
    fprintf(stderr, "Invalid opcode in process %s\n",
            running->process_name.c_str());
    running = nullptr;
    break;
  case ReturnStatus_t::READ_OPERATION :
#ifdef DEBUG
    printf("Process %s encountered read operation\n",
           running->process_name.c_str());
#endif  // DEBUG
    io_register = getIORegister(running);
    running->in_file >> running->r[io_register];
    running->interrupt_time = system_time + IO_SYSTEM_TIME;
    waiting.push(std::move(running));
    break;
  case ReturnStatus_t::WRITE_OPERATION :
#ifdef DEBUG
    printf("Process %s encountered write operation\n",
        running->process_name.c_str());
#endif  // DEBUG
    io_register = getIORegister(running);
    running->out_file << running->r[io_register] << std::endl;
    running->interrupt_time = system_time + IO_SYSTEM_TIME;
    waiting.push(std::move(running));
    break;
  }
}


/**
 * Get the return status of the given PCB.
 * @param  pcb -> The PCB being checked.
 * @return the status of that PCB
 */
inline uint8_t OS::getReturnStatus(const PCB* pcb) const {
  return ((pcb->sr &
           (RETURN_STATUS_MASK << RETURN_STATUS_SHIFT)) >> RETURN_STATUS_SHIFT);
}


/**
 * Get the I/O register of the given PCB.
 * @param  pcb -> The PCB being checked.
 * @return the IO register of that PCB.
 */
inline uint8_t OS::getIORegister(const PCB* pcb) const {
  return ((pcb->sr &
           (IO_REGISTER_MASK << IO_REGISTER_SHIFT)) >> IO_REGISTER_SHIFT);
}


/**
 * Find all *.s files and load their names (and paths) into memory
 */
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
