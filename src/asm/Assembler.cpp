/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./Assembler.h"
#include <err/Errors.h>
#include <cstdint>
#include <sys/stat.h>
#include <list>
#include <string>
#include <iostream>

#define EXTENSION_SEPARATOR     "."
#define ASSEMBLY_FILE_EXTENSION ".s"


Assembler::Assembler() {}


std::string Assembler::parse(std::string file_name) {
  checkFileName(file_name);
  checkFileExists(file_name);
  return std::string();
}


void Assembler::checkFileName(std::string file_name) {
  bool valid = isFileNameValid(file_name);
  try {
    if (!valid) {
      throw InvalidFileName();
    }
  } catch (InvalidFileName &e) {
    reportError(e);
  }
}


void Assembler::checkFileExists(std::string file_name) {
  bool exists = doesFileExist(file_name);
  try {
    if (!exists) {
      throw FileDoesNotExist();
    }
  } catch (FileDoesNotExist &e) {
    reportError(e);
  }
}


bool Assembler::isFileNameValid(std::string file_name) {
  size_t pos = file_name.find_last_of(EXTENSION_SEPARATOR);
  std::string extension = "";
  if (pos != std::string::npos) {
    extension = file_name.substr(pos);
  }
  return extension == ASSEMBLY_FILE_EXTENSION;
}


bool Assembler::doesFileExist(std::string file_name) {
  struct stat buffer;
  return (stat (file_name.c_str(), &buffer) == 0);
}


void Assembler::reportError(std::exception &e) {
  printf("Assembler error: %s. Terminating...\n", e.what());
}


