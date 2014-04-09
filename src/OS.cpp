/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <asm/Assembler.h>
#include <vm/VirtualMachine.h>
#include <test/Test.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#define TEST_FLAG "--run-tests"

void printUsage(char *name) {
  printf("Usage: %s [--run-tests] <source file>\n", name);
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printUsage(argv[0]);
    exit(EXIT_FAILURE);
  }
  /*
  if (strncmp(argv[1], TEST_FLAG, strlen(TEST_FLAG))) {
    return runTests(argv[2]);
  }
  */
  Assembler a;
  VirtualMachine vm;
  std::string object_file_name = a.parse({argv[1]});
  if (object_file_name != "") {
    vm.run(object_file_name);
  }
  return 0;
}
