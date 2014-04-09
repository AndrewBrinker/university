/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./Assembler.h"
#include <err/Errors.h>
#include <sys/stat.h>
#include <cstdint>
#include <cstdlib>
#include <list>
#include <string>
#include <iostream>
#include <fstream>

#define EXTENSION_SEPARATOR     "."
#define ASSEMBLY_FILE_EXTENSION ".s"
#define OBJECT_FILE_EXTENSION   ".o"


/**
 * Empty constructor
 */
Assembler::Assembler() {}


/**
 * Parse the given file, reporting errors as necessary.
 *
 * This function first gets the file against a battery of tests. If it passes,
 * it will proceed to parse the file, otherwise it will report the error and
 * exit.
 *
 * @param  file_name -> The name of the file to be compiled.
 * @return the name of the created object file.
 */
std::string Assembler::parse(std::string file_name) {
  // Check whether the file name is valid
  try {
    if (!isFileNameValid(file_name)) {
      throw InvalidFileName();
    }
  } catch(std::exception &e) {
    reportError(e);
  }

  // Check whether the file exists
  try {
    if (!doesFileExist(file_name)) {
      throw FileDoesNotExist();
    }
  } catch(std::exception &e) {
    reportError(e);
  }

  // Check whether the file can be opened
  std::ifstream input_file(file_name);
  try {
    if (!input_file.is_open()) {
      throw CantOpenFile();
    }
  } catch(std::exception &e) {
    reportError(e);
  }

  // Check whether the output file can be made.
  std::string object_file_name = stripExtension(file_name);
  object_file_name += OBJECT_FILE_EXTENSION;
  std::ofstream output_file(object_file_name);
  try {
    if (!output_file.is_open()) {
      throw CantMakeFile();
    }
  } catch(std::exception &e) {
    reportError(e);
  }

  return std::string();
}


/**
 * Check whether the given file name is a valid assembly source file.
 *
 * The only standard for validity is that it ends with a ".s" extension. That is
 * what this function checks for.
 *
 * @param  file_name -> The name of the assembly source file being checked.
 * @return whether the name is valid or not.
 */
bool Assembler::isFileNameValid(std::string file_name) {
  size_t pos = file_name.find_last_of(EXTENSION_SEPARATOR);
  std::string extension = "";
  if (pos != std::string::npos) {
    extension = file_name.substr(pos);
  }
  return extension == ASSEMBLY_FILE_EXTENSION;
}


/**
 * Check whether the given file name exists.
 * @param  file_name -> The name of the file being checked.
 * @return whether the file exists.
 */
bool Assembler::doesFileExist(std::string file_name) {
  struct stat buffer;
  return (stat (file_name.c_str(), &buffer) == 0);
}


/**
 * Report the given error and exit gracefully.
 * @param e -> The error being reported.
 */
void Assembler::reportError(std::exception &e) {
  printf("Assembler error: %s. Terminating...\n", e.what());
  exit(EXIT_FAILURE);
}


