/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

/*
#include <os/OS.h>

int main(int argc, char* argv[]) {
  OS os;
  os.run();
  return 0;
}
*/

#include <tlb/TLB.h>

#include <cstdio>

int main() {
  TLB_Entry te;

  printf( "%d\n", te.frame );

  return 0;
}
