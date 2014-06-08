/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <os/OS.h>
#include <cstdio>
#include <cstring>

int main(int argc, char* argv[]) {
  OS os;

  if (argc != 2) {
    printf("Usage: %s -lru|-fifo\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (strncmp(argv[1], "-lru", 4) == 0) {
    os.run(LRU_ALGO);
  } else if (strncmp(argv[1], "-fifo", 5) == 0) {
    os.run(FIFO_ALGO);
  } else {
    printf("Error: Method not recognized. Please try again.\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}
