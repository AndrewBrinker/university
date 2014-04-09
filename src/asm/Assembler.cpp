/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./Assembler.h"
#include <err/Errors.h>
#include <sys/stat.h>
#include <cstdint>
#include <cstdlib>
#include <cctype>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define EXTENSION_SEPARATOR     "."
#define COMMENT_SEPARATOR       "!"
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

  // Get the assembly file source
  ASMSource source = readASMSource(input_file);

  /*
  for (auto line : source) {
    printf("%s\n", line.c_str());
  }
  */

  // Go line by line
  // Split each line into a vector of components
  // Create a string for the output line
  for (auto line : source) {
    std::vector<std::string> parts = split(line);
    for (auto item : parts) {
      printf("%s ", item.c_str());
    }
    printf("\n");
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


/**
 * Remove the file extension from the given file name
 * @param  file_name -> The file name being stripped.
 * @return the stripped file name.
 */
std::string Assembler::stripExtension(std::string file_name) {
  size_t pos = file_name.find_last_of(EXTENSION_SEPARATOR);
  if (pos != std::string::npos) {
    return file_name.substr(0, pos);
  }
  return file_name;
}


/**
 * Remove any comments from the given line of code
 * @param  line -> The line to be stripped.
 * @return the stripped line.
 */
std::string Assembler::stripComments(std::string line) {
  size_t pos = line.find_last_of(COMMENT_SEPARATOR);
  if (pos != std::string::npos) {
    return line.substr(0, pos);
  }
  return line;
}


std::string Assembler::stripEndingWhitespace(std::string line) {
  int i = line.length();
  do {
    --i;
  } while (isspace(line[i]));
  return line.substr(0, i + 1);
}


/**
 * Split the given string into a vector of its space-delimited parts
 * @param line -> The line to be split.
 * @return the split line.
 */
std::vector<std::string> Assembler::split(std::string line) {
  std::vector<std::string> result;
  size_t j = 0;
  size_t i = 0;
  while (i < line.length()) {
    if (isspace(line[i])) {
      result.push_back(line.substr(j, i - j));
      j = i + 1;
    }
    ++i;
  }
  result.push_back(line.substr(j, i));
  return result;
}


/**
 * Convert the input file stream into ASMSource
 * @param  input_file -> The stream to the assembly file being read
 * @return the source of the assembly file
 */
Assembler::ASMSource Assembler::readASMSource(std::ifstream &input_file) {
  ASMSource source;
  for (std::string line; getline(input_file, line);) {
      line = stripComments(line);
      line = stripEndingWhitespace(line);
      if (line == "\n") continue;
      source.push_back(line);
  }
  return source;
}


