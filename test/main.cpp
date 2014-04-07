#include "../src/vm/VirtualMachine.h"
#include <cstdio>
#include <cstdlib>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s [obj file]\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  VirtualMachine vm;
  vm.run(argv[1]);
  return 0;
}
