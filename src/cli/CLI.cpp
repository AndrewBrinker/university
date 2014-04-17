/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./CLI.h"
#include <asm/Assembler.h>
#include <vm/VirtualMachine.h>
#include <cstdio>
#include <map>
#include <string>

#define CURRENT_VERSION "0.1"


CLI::CLI(const int argc, char **argv) {
  // Valid args:
  //   --help    | -h
  //   --version | -V
  //   --verbose | -v
  //   --quiet   | -q
  // Everything else is treated as a file name.
  for (int i = 1; i < argc; ++i) {
    const std::string current = std::string(argv[i]);
    if (current == "--help" || current == "-h") {
      args["help"] = true;
    } else if (current == "--version" || current == "-v") {
      args["version"] = true;
    } else {
      files.push_back(current);
    }
  }
}


void CLI::parse() {
  if (args.count("help") || args.size() == 0) {
    printHelp();
    return;
  }
  if (args.count("version")) {
    printVersion();
    return;
  }
  Assembler a;
  VirtualMachine vm;
  for (std::string file : files) {
    std::string object_file = a.parse(file);
    if (object_file != "") {
      vm.run(object_file);
    }
  }
}


void CLI::printHelp() {
  printf("Usage: ./os [--help | -h] [--version | -v] <file> [file...]\n");
}


void CLI::printVersion() {
  printf("Version %s\n", CURRENT_VERSION);
}
