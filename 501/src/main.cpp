/**
 * Copyright 2015 Andrew Brinker
 */

#include <transition_table.h>
#include <cstdlib>
#include <cstdio>

int main(int argc, char **argv) {
  if (argc != 2) {
    crash("Usage: ./fa_to_regex <fa_file_name>");
  }

  transition_table table = load({argv[1]});
  printf("%s\n", fa_to_regex(table).c_str());
}
