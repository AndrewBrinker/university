/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */


#include <os/OS.h>
#include <cstdio>
#include <cstring>

int main(int argc, char* argv[]) {
  OS os;

  if (argc == 2) {
    if (strcmp(argv[1], "lru") == 0) {
    os.run("lru");
    } else if (strcmp(argv[1], "fifo") == 0) {
      os.run("fifo");
    }
  } else {
    printf("Format: %s (lru|fifo)\n", argv[0]);
  }

  return 0;
}
