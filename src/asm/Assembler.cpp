/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./Assembler.h"
#include <cstdint>
#include <list>
#include <err/Error.h>


Assembler::Assembler() {}


Assembler::ObjectSource Assembler::parse(char *) {
  return std::list<uint16_t>();
}
