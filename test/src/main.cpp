/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "../../src/vm/VirtualMachine.h"
#include "../lib/catch.h"
#include <cstdlib>
#include <cstdio>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <object file name>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  VirtualMachine vm;
  vm.run(argv[1]);
  return 0;
}
