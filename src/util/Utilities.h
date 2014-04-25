/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <vector>

#define EXT_SEP       "."
#define COMMENT_SEP   "!"
#define ASM_FILE_EXT  ".s"
#define OBJ_FILE_EXT  ".o"
#define ASM_MODE      0
#define VM_MODE       1


/**
 * Check whether the given file name is a valid assembly source file.
 *
 * The only standard for validity is that it ends with a ".s" extension. That is
 * what this function checks for.
 *
 * @param  file_name -> The name of the assembly source file being checked.
 * @return whether the name is valid or not.
 */
bool isFileNameValid(const std::string file_name, const int mode);


/**
 * Check whether the given file name exists.
 * @param  file_name -> The name of the file being checked.
 * @return whether the file exists.
 */
bool doesFileExist(std::string file_name);


/**
 * Remove the file extension from the given file name
 * @param  file_name -> The file name being stripped.
 * @return the stripped file name.
 */
std::string stripExtension(std::string file_name);


/**
 * Remove the path from the given file name
 * @param  file_name -> The file name being stripped.
 * @return the stripped file name.
 */
std::string stripPath(std::string file_name);


/**
 * Pad the given line with the fill character up to the given size
 * @param line        -> The line to be padded
 * @param fill        -> The filler character to use
 * @param target_size -> The target size
 */
void pad(std::string *line, const char fill, size_t target_size);


/**
 * Convert a given integer into its signed binary string representation
 * @param  original -> The integer to be converted
 * @param  bits     -> The number of bits to use (including a bit for the sign)
 * @return either the converted string or an empty string
 */
std::string toSignedBinaryString(const int original, const int bits);


/**
 * Convert a given integer into its unsigned binary string representation.
 * @param  original -> The integer to be converted
 * @param  bits     -> The number of bits to use
 * @return either the converted string, or an empty string
 */
std::string toUnsignedBinaryString(const int original, const int bits);


/**
 * Convert the given binary string into a decimal string of the same number
 * @param  original -> The string to be converted
 * @return the converted string
 */
int binaryStringToDecimal(const std::string original);


/**
 * Remove any comments from the given line of code
 * @param  line -> The line to be stripped.
 * @return the stripped line.
 */
std::string stripComments(std::string line);


/**
 * Remove whitespace from the end of a string
 * @param  line -> The string to be stripped
 * @return the stripped string
 */
std::string stripEndingWhitespace(std::string line);


/**
 * Split the given string into a vector of its space-delimited parts
 * @param line -> The line to be split.
 * @return the split line.
 */
std::vector<std::string> split(std::string line);


/**
 * Convert the given integer into a binary string represenation
 * @param  n    -> The integer being converted
 * @param  size -> The size of the output string
 * @return the binary string representation of the integer
 */
std::string bin(uintmax_t n, uintmax_t size = 0);


/**
 * Convert the given integer into a hexadecimal string representation
 * @param  n    -> The integer being converted
 * @param  size -> The size of the output string
 * @return the hexadecimal string representation of the integer
 */
std::string hex(uintmax_t n, uintmax_t size = 0);

/**
 * Check to see if the given string has the second string as a suffix.
 * @param str -> The string
 * @param suf -> The suffix
 * @return whether suf is a suffix of str
 */
bool has_suffix(std::string str, std::string suf);

#endif  // UTILITIES_H
