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
#include <iostream>
#include <algorithm>
#include <iterator>

#define FIFO_ALGO 0
#define LRU_ALGO  1

#define MULT_LIMIT          5
#define N_REGISTERS         4
#define TIME_SLICE_TIME     15
#define RETURN_STATUS_MASK  07
#define RETURN_STATUS_SHIFT 5
#define IO_REGISTER_MASK    03
#define IO_REGISTER_SHIFT   8
#define IO_DURATION         28


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
 * @param algo_id -> The page replacement algorithm to be used. Either LRU or FIFO
 */
void OS::run(int algo_id) {
  algo = algo_id;
  int elapsed_time = 0;

  // Get the source files
  std::vector<std::string> source_files = getSourceFiles();

  // Load the programs and prep them for execution
  std::unique_ptr<PCB> current_pcb;
  for (std::string source_file : source_files) {
    // Create a PCB for the file
    current_pcb = std::unique_ptr<PCB>(new PCB(source_file));

    // Parse the assembly
    as->parse(current_pcb.get());

    // Load the assembly into memory
    vm->loadIntoMemory(current_pcb.get());

    // Push the PCB onto the ready queue
    if (ready.size() < MULT_LIMIT) {
      ready.push(current_pcb.get());
    }

    // Push the PCB onto the jobs queue
    jobs.push_back(std::move(current_pcb));
  }

  // Start running the first process
  assignNextProcess();

  // Run all processes until they're finished
  while (running) {
#ifdef DEBUG
    std::cout << "Running process: " << running->name << std::endl;
#endif  // DEBUG
    elapsed_time      = vm->runProcess(running, TIME_SLICE_TIME);
    system_time      += elapsed_time;
    running->vm_time += elapsed_time;
    contextSwitch();
  }

  // Calculuate system utilization
  double sys_utilization = system_time - idle_time;
  sys_utilization /= static_cast<double>(system_time);

  // Calculuate user utilization
  double user_utilization = 0.0;
  for (std::unique_ptr<PCB>& pcb : jobs) {
    user_utilization += pcb->vm_time;
  }
  user_utilization /= system_time;

  // Calculate throughput
  double throughput = jobs.size() * 1000.0 / system_time;

  // Output all accounting information
  for (std::unique_ptr<PCB>& pcb : jobs) {
    // Process-specific accounting information
    pcb->out_file << "VM Time:            " << pcb->vm_time            << "\n"
                  << "Waiting Time:       " << pcb->waiting_time       << "\n"
                  << "Turnaround Time:    " << pcb->turnaround_time    << "\n"
                  << "I/O Time:           " << pcb->io_time            << "\n"
                  << "Largest Stack Size: " << pcb->largest_stack      << "\n"
                  << std::endl;

    // System accounting information
    pcb->out_file << "System CPU Utilization: " << sys_utilization  << "\n"
                  << "User CPU Utilization:   " << user_utilization << "\n"
                  << "Throughput:             " << throughput       << "\n"
                  << std::endl;
  }
}


/**
 * If there is a ready process, set it as the running process and remove it from
 * the ready queue.
 *
 * Otherwise, add a process from the waiting queue to the ready queue and then
 * try again to run a ready process.
 */
void OS::assignNextProcess() {
  PCB* temp;
  if (!ready.empty()) {
    running = const_cast<PCB*>(ready.front());
    ready.pop();
    running->waiting_time += system_time - running->time_entered_ready_queue;
  } else if (!waiting.empty()) {
    idle_time  += waiting.front()->interrupt_time - system_time;
    system_time = waiting.front()->interrupt_time;
    temp = const_cast<PCB*>(waiting.front());
    waiting.pop();
    temp->io_time += system_time - temp->time_entered_waiting_queue;
    temp->time_entered_ready_queue = system_time;
    ready.push(temp);
    temp = nullptr;
    assignNextProcess();
  }
}


/**
 * Perform a context switch.
 */
void OS::contextSwitch() {
  PCB* temp;
  // First, all the processes in wait whose I/O operation has been completed are
  // placed in ready
  while (!waiting.empty() && waiting.front()->interrupt_time <= system_time) {
    temp = const_cast<PCB*>(waiting.front());
    waiting.pop();
    temp->io_time += system_time - temp->time_entered_waiting_queue;
    temp->time_entered_ready_queue = system_time;
    ready.push(temp);
    temp = nullptr;
  }

  // Second the running process is placed in the proper queue or terminated, and
  return_t status = static_cast<return_t>(getReturnStatus(running));
  uint8_t io_register;
  switch (status) {
    case return_t::TIME_SLICE:
      // If it's a time slice, update the time and move the running
      // process onto the ready queue.
      running->time_entered_ready_queue = system_time;
      ready.push(std::move(running));
      break;
    case return_t::HALT_INSTRUCTION:
      // If it's halted, set the turnaround time and invalidate the pointer
      running->turnaround_time = system_time;
      running = nullptr;
      break;
    case return_t::OUT_OF_BOUND_REFERENCE:
      // If it's an out of bound reference, output the issue to stderr and
      // invalidate the pointer
      fprintf(stderr, "Out of bound reference in process %s\n",
              running->name.c_str());
      running = nullptr;
      break;
    case return_t::STACK_OVERFLOW:
      // If it's a stack overflow, output the issue to stderr and invalidate the
      // pointer
      fprintf(stderr, "Stack overflow in process %s\n",
              running->name.c_str());
      running = nullptr;
      break;
    case return_t::STACK_UNDERFLOW:
      // If it's a stack underflow, output the issue to stderr and invalidate
      // the pointer
      fprintf(stderr, "Stack underflow in process %s\n",
              running->name.c_str());
      running = nullptr;
      break;
    case return_t::INVALID_OPCODE:
      // If it's an invalid opcode, output the issue to stderr and invalidate
      // the pointer
      fprintf(stderr, "Invalid opcode in process %s\n",
              running->name.c_str());
      running = nullptr;
      break;
    case return_t::READ_OPERATION:
      // If it's a read operation, get the index of the IO register for the
      // running process, read from the input file into it, update the time
      // trackers, and move the process onto the waiting queue.
      io_register = getIORegister(running);
      running->in_file >> running->r[io_register];
      running->interrupt_time = system_time + IO_DURATION;
      running->time_entered_waiting_queue = system_time;
      waiting.push(std::move(running));
      break;
    case return_t::WRITE_OPERATION:
      // If it's a write operation, get the index of the IO register for the
      // running process, write to the output file from it, update the time
      // trackers, and move the process onto the waiting queue.
      io_register = getIORegister(running);
      running->out_file << running->r[io_register] << std::endl;
      running->interrupt_time = system_time + IO_DURATION;
      running->time_entered_waiting_queue = system_time;
      waiting.push(std::move(running));
      break;
  }

  // Third the next process for ready is assigned to the VM
  assignNextProcess();

  // A context switch takes 5 ticks of system time
  system_time += 5;
}


/**
 * Pick the victim frame for page replacement according to the current page
 * replacement algorithm.
 * @return the index for the frame to be replaced
 */
int OS::pickVictimFrame() {
  int victim = 0;
  uint16_t ticks = vm->getClock();
  if (algo == FIFO_ALGO) {
    // The OS's frame registers keep track of when a page was brought into
    // memory. This searches through them to find the oldest one, and returns
    // its index.
    for (int i = 0; i < N_FRAMES; ++i) {
      if ((ticks - frame_registers[i]) < (ticks - frame_registers[victim])) {
        victim = i;
      }
    }
  } else {
    // The VM's frame registers keep track of the time of the last page access.
    // This searches through them to find the oldest one, and returns its index.
    auto regs = vm->getFrameRegisters();
    for (int i = 0; i < N_FRAMES; ++i) {
      if ((ticks - regs[i]) < (ticks - regs[victim])) {
        victim = i;
      }
    }
  }
  return victim;
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
  std::vector<std::string> file_names;
  for (auto directory : walk()) {
    for (std::string file_name : directory.files) {
      if (hasSuffix(file_name, ".s")) {
        file_names.push_back(file_name);
      }
    }
  }
  return file_names;
}
