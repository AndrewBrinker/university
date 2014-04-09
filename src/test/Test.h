/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <vm/VirtualMachine.h>
#include <cstdlib>
#include <cstdio>
#include "../../lib/catch.h"

int runTests(char *name) {
  VirtualMachine vm;
  vm.run(name);
  return 0;
}
