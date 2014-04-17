/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include <cli/CLI.h>

int main(int argc, char **argv) {
  CLI c(argc, argv);
  c.parse();
  return 0;
}
