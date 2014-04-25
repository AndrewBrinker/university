/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./OS.h"

#include <string>
#include <vector>
#include <cstdio>
#include <memory>   // std::unique_ptr
#include <utility>  // std::move

#include <dir/Dir.h>
#include <util/Utilities.h>

#define N_REGISTERS 4

OS::OS() :
  as(new Assembler),
  vm(new VirtualMachine)
{}

void OS::run() {
  // Find all *.s files and load their names (and paths) into memory
  std::vector<std::string> source_files = get_sourcefiles();

#ifdef DEBUG
    for (std::string source_file : source_files)
      puts(source_file.c_str());
#endif  // DEBUG

    // Assemble each file, load it into memory, create its PCB, and put into
    // the ready queue
    std::string object_file;
    std::unique_ptr<PCB> curr_pcb;

    for (std::string source_file : source_files) {
      curr_pcb = std::unique_ptr<PCB>(new PCB(source_file));
      as->parse(curr_pcb.get());
      vm->load(curr_pcb.get());
      ready.push(std::move(curr_pcb));
    }

#ifdef DEBUG
    PCB* front = ready.front().get();
    do {
      puts(ready.front()->process_name.c_str());
      ready.push(std::move(ready.front()));
      ready.pop();
    } while (front != ready.front().get());
#endif  // DEBUG
}

// Find all *.s files and load their names (and paths) into memory
std::vector<std::string> OS::get_sourcefiles() {
  std::vector<std::string> filenames;
  for (auto de : walk())
    for (std::string filename : de.files)
      if (has_suffix(filename, ".s"))
        filenames.push_back(filename);

  return filenames;
}
