/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./CLI.h"
#include <asm/Assembler.h>
#include <vm/VirtualMachine.h>
#include <dbg/Debug.h>
#include <cstdio>
#include <map>
#include <string>
#include <iostream>

#define CURRENT_VERSION "0.1"

/**
 * Load the contents of argv into args and files, respectively.
 * @param argc -> The size of argv
 * @param argv -> The arguments themselves
 */
CLI::CLI(const int argc, char **argv) {
  // Valid args:
  //   --help        | -h
  //   --version     | -V
  //   --interactive | -i
  //   --debug       | -d
  // Everything else is treated as a file name.
  for (int i = 1; i < argc; ++i) {
    const std::string current = std::string(argv[i]);
    if (current == "--help" || current == "-h") {
      args["help"] = true;
    } else if (current == "--version" || current == "-v") {
      args["version"] = true;
    } else if (current == "--interactive" || current == "-i") {
      args["interactive"] = true;
    } else if (current == "--debug" || current == "-d") {
      args["debug"] = true;
    } else {
      files.push_back(current);
    }
  }
}


/**
 * Process the given arguments appropriately
 */
void CLI::parse() {
  if (args.count("help") || (args.size() == 0 && files.size() == 0)) {
    printHelp();
    return;
  }
  if (args.count("version")) {
    printVersion();
    return;
  }
  if (args.count("interactive")) {
    startInteractiveSession();
    return;
  }
  if (args.count("debug")) {
    Debug::Instance().on = true;
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


/**
 * Print the help message explaining how this thing works
 */
void CLI::printHelp() {
  printf("\n");
  printf("About:\n");
  printf("\tskagOS is a virtual operating system made for CSE 460 at CSUSB\n");
  printf("\tin the Spring of 2014 by Andrew Brinker and Anthony Sterrett.\n");
  printf("\n");
  printf("Usage:\n");
  printf("\t./os [flags] <file> [file...]\n");
  printf("\n");
  printf("Flags:\n");
  printf("\t-h | --help          Print this help text.\n");
  printf("\t-v | --version       Print the current version number.\n");
  printf("\t-i | --interactive   Open an interactive session.\n");
  printf("\t-d | --debug         Execute files in debug mode.\n");
  printf("\n");
}


/**
 * Print the current version number
 */
void CLI::printVersion() {
  printf("Version %s\n", CURRENT_VERSION);
}


/**
 * Open an interactive session
 */
void CLI::startInteractiveSession() {
  printf("Interactive Session Opened.\n");
  printf("You may input paths to assembly files to assemble and run them.\n");
  printf("To exit, type `exit`.\n");
  std::string input = "";
  while (true) {
    printf(" file > ");
    std::cin >> input;
    if (input == "exit") break;
    Assembler a;
    VirtualMachine vm;
    std::string object_file = a.parse(input);
    if (object_file != "") {
      vm.run(object_file);
    }
  }
  printf("Exiting interactive session...\n");
  return;
}

