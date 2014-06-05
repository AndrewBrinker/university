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
 */
void OS::run(std::string /*page_replacement_algorithm*/) {
  int elapsed_time;
  // Find all *.s files and load their names (and paths) into memory
  std::vector<std::string> source_files = getSourceFiles();

#ifdef DEBUG
  // Print the loaded processes
  std::cout << "Loaded processes:" << std::endl;
  std::copy(std::begin(source_files),
            std::end(source_files),
            std::ostream_iterator<std::string>(std::cout, "\n"));
  std::cout << std::endl;
#endif  // DEBUG

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

  assignNextProcess();
  while (running) {
    elapsed_time = vm->runProcess(running, TIME_SLICE_TIME);
    system_time += elapsed_time;
    running->vm_time += elapsed_time;
    contextSwitch();
  }

  // Output accounting information

  double sys_utilization = (system_time - idle_time) /
                           static_cast<double>(system_time);

  double user_utilization;
  user_utilization = 0.0;
  for (std::unique_ptr<PCB>& pcb : jobs) {
    user_utilization += pcb->vm_time;
  }
  user_utilization /= system_time;

  double throughput = jobs.size() * 1000.0 / system_time;

  for (std::unique_ptr<PCB>& pcb : jobs) {
    // Process-specific accounting information
    pcb->out_file << "VM Time: " << pcb->vm_time << std::endl <<
                     "Waiting Time: " << pcb->waiting_time << std::endl <<
                     "Turnaround Time: " << pcb->turnaround_time << std::endl <<
                     "I/O Time: " << pcb->io_time << std::endl <<
                     "Largest Stack Size: " << pcb->largest_stack_size <<
                     std::endl << std::endl;

    // System accounting information
    pcb->out_file << "System CPU Utilization: " << sys_utilization <<
                     std::endl <<
                     "User CPU Utilization: " << user_utilization <<
                     std::endl <<
                     "Throughput: " << throughput << std::endl;
  }
}


/**
 * Assign the next process from the ready queue to running
 */
void OS::assignNextProcess() {
  PCB* temp;
  if (!ready.empty()) {
    running = const_cast<PCB*>(ready.front());
    ready.pop();
    running->waiting_time += system_time - running->time_entered_ready_queue;
  } else if (!waiting.empty()) {
    idle_time += waiting.front()->interrupt_time - system_time;
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
  // first, all the processes in wait whose I/O operation has been completed are
  // placed in ready
  while (!waiting.empty() && waiting.front()->interrupt_time <= system_time) {
    temp = const_cast<PCB*>(waiting.front());
    waiting.pop();
    temp->io_time += system_time - temp->time_entered_waiting_queue;
    temp->time_entered_ready_queue = system_time;
    ready.push(temp);
    temp = nullptr;
  }

  // second the running process is placed in the proper queue or terminated, and
  ReturnStatus_t return_status =
    static_cast<ReturnStatus_t>(getReturnStatus(running));
  uint8_t io_register;
  switch (return_status) {
    case ReturnStatus_t::TIME_SLICE:
      running->time_entered_ready_queue = system_time;
      ready.push(std::move(running));
      break;
    case ReturnStatus_t::HALT_INSTRUCTION:
      running->turnaround_time = system_time;
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
      running->interrupt_time = system_time + IO_DURATION;
      running->time_entered_waiting_queue = system_time;
      waiting.push(std::move(running));
      break;
    case ReturnStatus_t::WRITE_OPERATION:
      io_register = getIORegister(running);
      running->out_file << running->r[io_register] << std::endl;
      running->interrupt_time = system_time + IO_DURATION;
      running->time_entered_waiting_queue = system_time;
      waiting.push(std::move(running));
      break;
  }

  // third the next process for ready is assigned to the VM
  assignNextProcess();

  // A context switch takes 5 ticks of system time
  system_time += 5;
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
