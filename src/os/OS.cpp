#include "./OS.h"
#include "../dir/Dir.h"
#include "../util/Utilities.h"

#include <string>
#include <vector>
#include <cstdio>
#include <memory>   // std::unique_ptr
#include <utility>  // std::move

#define N_REGISTERS 4

OS::OS() {}

void OS::run() {
  // Find all *.s files and load their names (and paths) into memory
  std::vector<std::string> source_files = get_sourcefiles();

#ifdef DEBUG
    for (std::string source_file : source_files)
      puts(source_file.c_str());
#endif  // DEBUG

    // Assemble each file, load it into memory, create its PCB, and put into the ready queue
    std::string object_file;
    std::unique_ptr<PCB> curr_pcb;

    for (std::string source_file : source_files) {
      object_file = as.parse(source_file);
      curr_pcb = std::unique_ptr<PCB>(new PCB);
      curr_pcb->pname = source_file;
      curr_pcb->r.resize(N_REGISTERS);
#ifdef DEBUG
      curr_pcb->asm_source = as.asm_source;
#endif  // DEBUG
      ready.push(std::move(curr_pcb));
    }

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
