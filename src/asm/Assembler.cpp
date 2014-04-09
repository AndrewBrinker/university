/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./Assembler.h"
#include <err/Errors.h>
#include <sys/stat.h>
#include <cstdint>
#include <cstdlib>
#include <cctype>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#define EXTENSION_SEPARATOR     "."
#define COMMENT_SEPARATOR       "!"
#define ASSEMBLY_FILE_EXTENSION ".s"
#define OBJECT_FILE_EXTENSION   ".o"
#define OBJECT_LINE_SIZE        16
#define REGS_FORMAT             0
#define CONST_FORMAT            1
#define ADDR_FORMAT             2
#define EMPTY_FORMAT            3
#define NOT_IMMEDIATE           "0"
#define IMMEDIATE               "1"
#define EMPTY_OP                {"","","",-1}

static const Assembler::op operations[] = {
  {"load"   , "00000", NOT_IMMEDIATE, CONST_FORMAT},
  {"loadi"  , "00000", IMMEDIATE    , CONST_FORMAT},
  {"store"  , "00001", NOT_IMMEDIATE, CONST_FORMAT},
  {"add"    , "00010", NOT_IMMEDIATE, REGS_FORMAT},
  {"addi"   , "00010", IMMEDIATE    , CONST_FORMAT},
  {"addc"   , "00011", NOT_IMMEDIATE, REGS_FORMAT},
  {"addci"  , "00011", IMMEDIATE    , CONST_FORMAT},
  {"sub"    , "00100", NOT_IMMEDIATE, REGS_FORMAT},
  {"subi"   , "00100", IMMEDIATE    , CONST_FORMAT},
  {"subc"   , "00101", NOT_IMMEDIATE, REGS_FORMAT},
  {"subci"  , "00101", IMMEDIATE    , CONST_FORMAT},
  {"and"    , "00110", NOT_IMMEDIATE, REGS_FORMAT},
  {"andi"   , "00110", IMMEDIATE    , CONST_FORMAT},
  {"xor"    , "00111", NOT_IMMEDIATE, REGS_FORMAT},
  {"xori"   , "00111", IMMEDIATE    , CONST_FORMAT},
  {"compl"  , "01000", NOT_IMMEDIATE, CONST_FORMAT},
  {"shl"    , "01001", NOT_IMMEDIATE, CONST_FORMAT},
  {"shla"   , "01010", NOT_IMMEDIATE, CONST_FORMAT},
  {"shr"    , "01011", NOT_IMMEDIATE, CONST_FORMAT},
  {"shra"   , "01100", NOT_IMMEDIATE, CONST_FORMAT},
  {"compr"  , "01101", NOT_IMMEDIATE, REGS_FORMAT},
  {"compri" , "01101", IMMEDIATE    , CONST_FORMAT},
  {"getstat", "01110", NOT_IMMEDIATE, CONST_FORMAT},
  {"putstat", "01111", NOT_IMMEDIATE, CONST_FORMAT},
  {"jump"   , "10000", NOT_IMMEDIATE, ADDR_FORMAT},
  {"jumpl"  , "10001", NOT_IMMEDIATE, ADDR_FORMAT},
  {"jumpe"  , "10010", NOT_IMMEDIATE, ADDR_FORMAT},
  {"jumpg"  , "10011", NOT_IMMEDIATE, ADDR_FORMAT},
  {"call"   , "10100", NOT_IMMEDIATE, ADDR_FORMAT},
  {"return" , "10101", NOT_IMMEDIATE, EMPTY_FORMAT},
  {"read"   , "10110", NOT_IMMEDIATE, CONST_FORMAT},
  {"write"  , "10111", NOT_IMMEDIATE, CONST_FORMAT},
  {"halt"   , "11000", NOT_IMMEDIATE, EMPTY_FORMAT},
  {"noop"   , "11001", NOT_IMMEDIATE, EMPTY_FORMAT}
};


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

  ObjectSource object_source;
  for (auto line : source) {
    object_source.push_back(convertToObjectCode(line));
  }

  for (auto line : object_source) {
    printf("%s\n", line.c_str());
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
    if (line == "") {
      continue;
    }
    source.push_back(line);
  }
  return source;
}


/**
 * Convert a given line of assembly code to object code
 * @param  line -> The line of assembly code to convert.
 * @return the object code created.
 */
std::string Assembler::convertToObjectCode(std::string line) {
  std::string object_line = "";
  std::vector<std::string> parts = split(line);
  op current_op = findOperation(parts[0]);
  object_line += current_op.op_code;
  switch (current_op.format) {
    case REGS_FORMAT:
      object_line += getRegisterID(parts[1]);
      // Do more stuff
      break;
    case CONST_FORMAT:
      object_line += getRegisterID(parts[1]);
      // Do more stuff.
      break;
    case ADDR_FORMAT:
      object_line += current_op.i;
      object_line += toBinaryString(atoi(parts[1].c_str()));
      pad(&object_line, '0', OBJECT_LINE_SIZE);
      break;
    case EMPTY_FORMAT:
      pad(&object_line, '0', OBJECT_LINE_SIZE);
      break;
  }
  return object_line;
}


/**
 * Get the operation identified by the given name
 * @param  name -> The name of the operation being searched for.
 * @return the associated operation.
 */
Assembler::op Assembler::findOperation(std::string name) {
  for (int i = 0; i < OP_COUNT; ++i) {
    if (operations[i].name == name) {
      return operations[i];
    }
  }
  try {
    throw InvalidOperation();
  } catch(std::exception &e) {
    reportError(e);
  }
  return EMPTY_OP;
}


/**
 * Get the register ID for the given string
 * @param id -> The ID identifying the register
 * @return the binary register ID
 */
std::string Assembler::getRegisterID(std::string id) {
  if (id == "0") return "00";
  if (id == "1") return "01";
  if (id == "2") return "10";
  if (id == "3") return "11";
  try {
    throw InvalidRegisterID();
  } catch(std::exception &e) {
    reportError(e);
  }
  return "";
}


/**
 * Pad the given line with the fill character up to the given size
 * @param line        -> The line to be padded
 * @param fill        -> The filler character to use
 * @param target_size -> The target size
 */
void Assembler::pad(std::string *line, const char fill, size_t target_size) {
  size_t length = line->length();
  if (length >= target_size) return;
  while (length < target_size) {
    line->push_back(fill);
    ++length;
  }
}


/**
 * Convert the given integer to a string of binary digits
 * @param  value -> the integer being converted.
 * @return the string binary encoding of that number.
 */
std::string Assembler::toBinaryString(const uint16_t value) {
  std::string output;
  bool found_first_one = false;
  for (int current_bit = 15; current_bit >= 0; current_bit--) {
    if ((value & (1ULL << current_bit)) != 0) {
      if (!found_first_one) {
        found_first_one = true;
      }
      output += '1';
    } else if (found_first_one) {
      output += '0';
    }
  }
  return output;
}


