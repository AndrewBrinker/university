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

#define TIME_SLICE 15

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
  system_time += vm->run_process(running.get(), TIME_SLICE);
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
