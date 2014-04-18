/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#include "./Assembler.h"
#include <err/Errors.h>
#include <util/Utilities.h>
#include <dbg/Debug.h>
#include <cstdint>
#include <cstdlib>
#include <cctype>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

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
#define ADDR_FMT        0
#define CONST_FMT       1
#define REGS_FMT        2
#define SHORT_REG_FMT   3
#define SHORT_ADDR_FMT  4
#define EMPTY_FMT       5

#define INVALID_FMT    -1
#define NOT_IMMEDIATE  "0"
#define IMMEDIATE      "1"
#define ADDR_BIT_COUNT  8
#define CONST_BIT_COUNT 8
#define OBJ_LINE_SIZE   16

static const Assembler::op_t operations[] = {
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
    if (!isFileNameValid(file_name, ASM_MODE)) {
      throw InvalidFileName("Assembler");
    }
  } catch(GenericError &e) {
    e.reportError();
  }

  // Check whether the file exists
  try {
    if (!doesFileExist(file_name)) {
      throw FileDoesNotExist("Assembler");
    }
  } catch(GenericError &e) {
    e.reportError();
  }

  // Check whether the file can be opened
  std::ifstream input_file(file_name);
  try {
    if (!input_file.is_open()) {
      throw CantOpenFile("Assembler");
    }
  } catch(GenericError &e) {
    e.reportError();
  }

  // Check whether the output file can be made.
  std::string object_file_name = stripExtension(file_name);
  object_file_name += OBJ_FILE_EXT;
  std::ofstream output_file(object_file_name);
  try {
    if (!output_file.is_open()) {
      throw CantMakeFile("Assembler");
    }
  } catch(GenericError &e) {
    e.reportError();
  }

  // Get the assembly file source
  std::vector<std::string> asm_source = readASMSource(input_file);

  // Convert it to object file source
  for (auto line : asm_source) {
    if (Debug::Instance().on) Debug::Instance().source.push_back(line);
    output_file << binaryStringToDecimal(convertToObjectCode(line)) << "\n";
  }

  // Close the file streams and return
  input_file.close();
  output_file.close();

  // Return the name of the file you output to
  return object_file_name;
}


/**
 * Convert the input file stream into ASMSource
 * @param  input_file -> The stream to the assembly file being read
 * @return the source of the assembly file
 */
std::vector<std::string> Assembler::readASMSource(std::ifstream &input_file) {
  std::vector<std::string> source;
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
  op_t current_op = findOperation(parts[0]);
  object_line += current_op.op_code;
  std::string result = "";
  switch (current_op.format) {
    case ADDR_FMT:
      object_line += getRegisterID(parts[1]);
      object_line += current_op.i;
      result = toUnsignedBinaryString(atoi(parts[2].c_str()), ADDR_BIT_COUNT);
      if (result != "") {
        object_line += result;
      } else {
        try {
          throw FailedIntegerConversion("Assembler");
        } catch(GenericError &e) {
          e.reportError();
        }
      }
      break;
    case CONST_FMT:
      object_line += getRegisterID(parts[1]);
      object_line += current_op.i;
      result = toSignedBinaryString(atoi(parts[2].c_str()), CONST_BIT_COUNT);
      if (result != "") {
        object_line += result;
      } else {
        try {
          throw FailedIntegerConversion("Assembler");
        } catch(GenericError &e) {
          e.reportError();
        }
      }
      break;
    case REGS_FMT:
      object_line += getRegisterID(parts[1]);
      object_line += current_op.i;
      object_line += getRegisterID(parts[2]);
      pad(&object_line, '0', OBJ_LINE_SIZE);
      break;
    case SHORT_REG_FMT:
      object_line += getRegisterID(parts[1]);
      object_line += current_op.i;
      pad(&object_line, '0', OBJ_LINE_SIZE);
      break;
    case SHORT_ADDR_FMT:
      object_line += getRegisterID("0");
      object_line += current_op.i;
      result = toUnsignedBinaryString(atoi(parts[1].c_str()), ADDR_BIT_COUNT);
      if (result != "") {
        object_line += result;
      } else {
        try {
          throw FailedIntegerConversion("Assembler");
        } catch(GenericError &e) {
          e.reportError();
        }
      }
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
Assembler::op_t Assembler::findOperation(std::string name) {
  for (auto operation : operations) {
    if (operation.name == name) {
      return operation;
    }
  }
  try {
    throw InvalidOperation("Assembler");
  } catch(GenericError &e) {
    e.reportError();
  }
  return {"", "", "", INVALID_FMT};
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
    throw InvalidRegisterID("Assembler");
  } catch(GenericError &e) {
    e.reportError();
  }
  return "";
}

