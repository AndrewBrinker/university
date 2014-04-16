/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./Assembler.h"
#include <err/Errors.h>
#include <sys/stat.h>
#include <cstdint>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

/**
 * Explanation of formats.
 *
 * cmd RD ADDR  -> ADDR_FMT
 * cmd RD CONST -> CONST_FMT
 * cmd RD RS    -> REGS_FMT
 * cmd RD       -> SHORT_REG_FMT
 * cmd ADDR     -> SHORT_ADDR_FMT
 * cmd          -> EMPTY_FMT
 */
#define ADDR_FMT       0
#define CONST_FMT      1
#define REGS_FMT       2
#define SHORT_REG_FMT  3
#define SHORT_ADDR_FMT 4
#define EMPTY_FMT      5
#define INVALID_FMT    -1

#define NOT_IMMEDIATE  "0"
#define IMMEDIATE      "1"
#define EMPTY_OP       {"", "", "", INVALID_FMT}
#define OP_COUNT       34

#define ADDR_BIT_COUNT 8

#define EXT_SEP        "."
#define COMMENT_SEP    "!"
#define ASM_FILE_EXT   ".s"
#define OBJ_FILE_EXT   ".o"
#define OBJ_LINE_SIZE  16

static const Assembler::op operations[] = {
  {"load"   , "00000", NOT_IMMEDIATE, ADDR_FMT       },
  {"loadi"  , "00000", IMMEDIATE    , CONST_FMT      },
  {"store"  , "00001", NOT_IMMEDIATE, ADDR_FMT       },
  {"add"    , "00010", NOT_IMMEDIATE, REGS_FMT       },
  {"addi"   , "00010", IMMEDIATE    , CONST_FMT      },
  {"addc"   , "00011", NOT_IMMEDIATE, REGS_FMT       },
  {"addci"  , "00011", IMMEDIATE    , CONST_FMT      },
  {"sub"    , "00100", NOT_IMMEDIATE, REGS_FMT       },
  {"subi"   , "00100", IMMEDIATE    , CONST_FMT      },
  {"subc"   , "00101", NOT_IMMEDIATE, REGS_FMT       },
  {"subci"  , "00101", IMMEDIATE    , CONST_FMT      },
  {"and"    , "00110", NOT_IMMEDIATE, REGS_FMT       },
  {"andi"   , "00110", IMMEDIATE    , CONST_FMT      },
  {"xor"    , "00111", NOT_IMMEDIATE, REGS_FMT       },
  {"xori"   , "00111", IMMEDIATE    , CONST_FMT      },
  {"compl"  , "01000", NOT_IMMEDIATE, SHORT_REG_FMT  },
  {"shl"    , "01001", NOT_IMMEDIATE, SHORT_REG_FMT  },
  {"shla"   , "01010", NOT_IMMEDIATE, SHORT_REG_FMT  },
  {"shr"    , "01011", NOT_IMMEDIATE, SHORT_REG_FMT  },
  {"shra"   , "01100", NOT_IMMEDIATE, SHORT_REG_FMT  },
  {"compr"  , "01101", NOT_IMMEDIATE, REGS_FMT       },
  {"compri" , "01101", IMMEDIATE    , CONST_FMT      },
  {"getstat", "01110", NOT_IMMEDIATE, SHORT_REG_FMT  },
  {"putstat", "01111", NOT_IMMEDIATE, SHORT_REG_FMT  },
  {"jump"   , "10000", NOT_IMMEDIATE, SHORT_ADDR_FMT },
  {"jumpl"  , "10001", NOT_IMMEDIATE, SHORT_ADDR_FMT },
  {"jumpe"  , "10010", NOT_IMMEDIATE, SHORT_ADDR_FMT },
  {"jumpg"  , "10011", NOT_IMMEDIATE, SHORT_ADDR_FMT },
  {"call"   , "10100", NOT_IMMEDIATE, SHORT_ADDR_FMT },
  {"return" , "10101", NOT_IMMEDIATE, EMPTY_FMT      },
  {"read"   , "10110", NOT_IMMEDIATE, SHORT_REG_FMT  },
  {"write"  , "10111", NOT_IMMEDIATE, SHORT_REG_FMT  },
  {"halt"   , "11000", NOT_IMMEDIATE, EMPTY_FMT      },
  {"noop"   , "11001", NOT_IMMEDIATE, EMPTY_FMT      }
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
  object_file_name += OBJ_FILE_EXT;
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

  return "";
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
  size_t pos = file_name.find_last_of(EXT_SEP);
  std::string extension = "";
  if (pos != std::string::npos) {
    extension = file_name.substr(pos);
  }
  return extension == ASM_FILE_EXT;
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
  size_t pos = file_name.find_last_of(EXT_SEP);
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
  size_t pos = line.find_last_of(COMMENT_SEP);
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
    case REGS_FMT:
      object_line += getRegisterID(parts[1]);
      object_line += current_op.i;
      object_line += getRegisterID(parts[2]);
      pad(&object_line, '0', OBJ_LINE_SIZE);
      break;
    case CONST_FMT:
      object_line += getRegisterID(parts[1]);
      object_line += current_op.i;
      // Do more stuff.
      break;
    case ADDR_FMT:
      object_line += current_op.i;
      object_line += toBinaryString(atoi(parts[1].c_str()), ADDR_BIT_COUNT);
      try {
        if (object_line == "") {
          throw InvalidAddress();
        }
      } catch(std::exception &e) {
        reportError(e);
      }
      pad(&object_line, '0', OBJ_LINE_SIZE);
      break;
    case EMPTY_FMT:
      pad(&object_line, '0', OBJ_LINE_SIZE);
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
 * Convert a given integer into its signed binary string representation
 * @param  original -> The integer to be converted
 * @param  bits     -> The number of bits to use (including a bit for the sign)
 * @return either the converted string or an empty string
 */
std::string toSignedBinaryString(const int original, const int bits) {
  // Check the sanity of the inputs.
  const float exponent = pow(2, bits - 1);
  if (original < -exponent || exponent - 1 < original || bits > 64) return "";
  // Do the conversion.
  std::string converted = "";
  bool first = false;
  for (int i = bits - 1; i >= 0; --i) {
    if ((original & (1ULL << i)) != 0) {
      if (!first) first = true;
      converted += '1';
    } else if (first) {
      converted += '0';
    }
  }
  // Pad the front by appending to the end of the string and then rotating the
  // string appropriately.
  const size_t size = converted.size();
  for (size_t i = 0; i < bits - size; ++i) converted.push_back('0');
  std::rotate(converted.begin(), converted.begin() + size, converted.end());
  return converted;
}


/**
 * Convert a given integer into its unsigned binary string representation.
 * @param  original -> The integer to be converted
 * @param  bits     -> The number of bits to use
 * @return either the converted string, or an empty string
 */
std::string toUnsignedBinaryString(const int original, const int bits) {
  std::string s = toSignedBinaryString(original, bits + 1);
  if (s[0] != '0') return "";
  return s.substr(1);
}


