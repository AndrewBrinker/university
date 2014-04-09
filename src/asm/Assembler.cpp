/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./Assembler.h"
#include <err/Error.h>
#include <cstdint>
#include <list>
#include <string>
#include <iostream>

#define EXTENSION_SEPARATOR     "."
#define ASSEMBLY_FILE_EXTENSION ".s"

Assembler::Assembler() {}


std::string Assembler::parse(std::string file_name) {
  checkFileName(file_name);
  return std::string();
}


void Assembler::checkFileName(std::string file_name) {
  size_t pos = file_name.find_last_of(EXTENSION_SEPARATOR);
  std::string extension = "";
  if (pos != std::string::npos) {
    extension = file_name.substr(pos);
  }
  try {
    if (extension != ASSEMBLY_FILE_EXTENSION) {
      throw InvalidFileName();
    }
  } catch (InvalidFileName& e) {
    reportError(e);
  }
}


void Assembler::reportError(std::exception &e) {
  printf("Assembler error: %s. Terminating...\n", e.what());
}


