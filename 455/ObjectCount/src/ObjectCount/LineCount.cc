/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 * Assignment: 2A
 *
 * Copyright 2014 Andrew Brinker
 */

#include "./LineCount.h"
#include <sys/stat.h>
#include <string>
#include <istream>
#include <fstream>
#include <algorithm>

namespace lc {

/** Sets the internal variables to their default values.
 *
 * This avoids uninitiated state problems that may otherwise arise, and makes
 * sure that the internal variables are always set to the appropriate values at
 * the creation of the object.
 */

LineCount::LineCount() : _is_good(true), _loc(0) {}


/** Alias for scan()
 *
 * Just calls scan().
 */

void LineCount::operator<<(std::string file_name) {
  scan(file_name);
  return;
}


/** Counts the number of lines of code in the given filename.
 *
 * Extracts the content of the passed-in stream and assumes the given value is
 * a filename. It attempts to open the file, and then begins scanning
 * line-by-line, counting the number of logical lines (defined as the number of
 * semicolons outside of comments) in the code. If it fails at any point the
 * function will exit and set `is_good` to false.
 */

void LineCount::scan(std::string file_name) {
  if (!fileExists(file_name)) {
    _is_good = false;
    return;
  }
  if ((!_is_good) || _loc != 0) {
    _is_good = false;
    return;
  }
  std::ifstream input_file(file_name);
  for (std::string line; getline(input_file, line);) {
    if (lineIsComment(line)) continue;
    if (lineHasSemicolon(line)) ++_loc;
  }
  return;
}


/** Clear all internal values.
 *
 * This function must be called before the same instance may be used again. It
 * clears all internal variables to prepare the instance for reuse. The input
 * function will check whether these values are at their defaults. If they are
 * not it will terminate and set `is_good` to false.
 */

void LineCount::clear() {
  _is_good = true;
  _loc = 0;
}


/** Return the value of the is_good flag.
 *
 * The is_good flag is the universal flag for whether the last file scanning was
 * successful. It does not provide any information beyond that. This function
 * simply returns the current value of that flag.
 */

bool LineCount::good() {
  return _is_good;
}


/** Return the negation of the is_good flag.
 *
 * The exact opposite of good(). This is provided simply for programmer
 * convenience.
 */

bool LineCount::bad() {
  return !_is_good;
}


/** Return the last-calculated number of lines-of-code.
 *
 * This assumes that the scanning has been performed at least once, and that the
 * clear() function has not been called since that time. If it has, this
 * function will return 0.
 */

unsigned LineCount::loc() {
  return _loc;
}


/** Checks whether a file with the given name exists or not.
 *
 * Uses an efficient implementation with stat() to determine whether or not a
 * given file exists.
 */

bool LineCount::fileExists(std::string file_name) {
  struct stat buffer;
  return (stat (file_name.c_str(), &buffer) == 0);
}


/** Checks whether the given line is a comment.
 *
 * The code has to account for both single line and multi-line comments, but it
 * should work correctly and allow the instance to properly ignore semicolons
 * inside of comments.
 */

bool LineCount::lineIsComment(std::string line) {
  trim(&line);
  std::string first_two = line.substr(0, 2);
  if (first_two == "//") {
    return true;
  } else if (first_two == "/*") {
    _in_comment = true;
    return true;
  } else if (first_two == "*/") {
    _in_comment = false;
    return false;
  } else if (first_two.substr(0, 1) == "*" && _in_comment == true) {
    return true;
  }
  return false;
}


/** Checks whether the given line has a semicolon.
 *
 * The string is passed as a constant reference to avoid performance issues with
 * the copying of large lines of code.
 */

bool LineCount::lineHasSemicolon(const std::string &line) {
  return (line.find(";") != std::string::npos);
}


/** Removes all whitespace from the given string.
 *
 * Modifies whatever is passed into it, so be careful to only pass in something
 * you want modified.
 */

void LineCount::trim(std::string *str) {
  str->erase(remove_if(str->begin(), str->end(), isspace), str->end());
}

}  // End namespace lc
