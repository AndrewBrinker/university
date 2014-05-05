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


#define N_REGISTERS         4
#define TIME_SLICE_TIME     15
#define RETURN_STATUS_MASK  07
#define RETURN_STATUS_SHIFT 5
#define IO_REGISTER_MASK    03
#define IO_REGISTER_SHIFT   8
#define IO_SYSTEM_TIME      27


/**
 * Initialize the internal variables.
 */
OS::OS()
  : as(new Assembler),
    vm(new VirtualMachine),
    system_time(0),
    idle_time(0) {}


/**
 * Load all the assembly files, create the PCBs, and run them all with
 * time-sharing.
 */
void OS::run() {
  // Find all *.s files and load their names (and paths) into memory
  std::vector<std::string> source_files = getSourceFiles();

  // Assemble each file, load it into memory, create its PCB, and put into
  // the ready queue
  std::unique_ptr<PCB> current_pcb;

  for (std::string source_file : source_files) {
    current_pcb = std::unique_ptr<PCB>(new PCB(source_file));
    as->parse(current_pcb.get());
    vm->loadIntoMemory(current_pcb.get());
    ready.push(current_pcb.get());
    jobs.push_back(std::move(current_pcb));
  }

  while (!ready.empty() || !waiting.empty()) {
    // If IO is complete
    if (!waiting.empty()) {
      if (waiting.front()->interrupt_time <= system_time) {
        ready.push(waiting.front());
        waiting.pop();
      }
    }
    if (!ready.empty()) {
      runNextProcess();
    } else {
      idle_time += waiting.front()->interrupt_time - system_time;
      system_time = waiting.front()->interrupt_time;
    }
  }
}


/**
 * Load the next process into the Virtual Machine and run it for its time slice.
 */
void OS::runNextProcess() {
  running = const_cast<PCB*>(ready.front());
  ready.pop();
  system_time += vm->runProcess(running, TIME_SLICE_TIME);

  ReturnStatus_t return_status =
    static_cast<ReturnStatus_t>(getReturnStatus(running));
  uint8_t io_register;
  switch (return_status) {
    case ReturnStatus_t::TIME_SLICE:
      ready.push(std::move(running));
      break;
    case ReturnStatus_t::HALT_INSTRUCTION:
      running = nullptr;
      break;
    case ReturnStatus_t::OUT_OF_BOUND_REFERENCE:
      fprintf(stderr, "Out of bound reference in process %s\n",
              running->process_name.c_str());
      running = nullptr;
      break;
    case ReturnStatus_t::STACK_OVERFLOW:
      fprintf(stderr, "Stack overflow in process %s\n",
              running->process_name.c_str());
      running = nullptr;
      break;
    case ReturnStatus_t::STACK_UNDERFLOW:
      fprintf(stderr, "Stack underflow in process %s\n",
              running->process_name.c_str());
      running = nullptr;
      break;
    case ReturnStatus_t::INVALID_OPCODE:
      fprintf(stderr, "Invalid opcode in process %s\n",
              running->process_name.c_str());
      running = nullptr;
      break;
    case ReturnStatus_t::READ_OPERATION:
      io_register = getIORegister(running);
      running->in_file >> running->r[io_register];
      running->interrupt_time = system_time + IO_SYSTEM_TIME;
      waiting.push(std::move(running));
      break;
    case ReturnStatus_t::WRITE_OPERATION:
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
std::vector<std::string> OS::getSourceFiles() {
  std::vector<std::string> filenames;
  for (auto de : walk()) {
    for (std::string filename : de.files) {
      if (hasSuffix(filename, ".s")) {
        filenames.push_back(filename);
      }
    }
  }
  return filenames;
}
