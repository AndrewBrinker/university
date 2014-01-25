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
  std::istream file(file_name);

  /*
   * Otherwise, go line by line. If the line is a comment, skip it. Otherwise,
   * check whether it has a semi-colon and update the count appropriately.
   */
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

}  // End namespace lc
