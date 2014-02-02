/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 * Assignment: 2A
 *
 * Copyright 2014 Andrew Brinker
 */

#include "./ObjectCount.h"
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

ObjectCount::ObjectCount() : LineCount(), _method_count(0) {}


/*
 * Strip file extension, if present
 * Search for both .h and .cc file
 * If they are not both present, exit and set the flags accordingly
 * If they are both present scan them both
 * The object LOC is LOC(.cc) + LOC(.h)
 * The # of method is LOC(.h)
 *
 */
void ObjectCount::scan(std::string file_name) {
  removeExtension(&file_name);
  std::string implementation_file_name = file_name + ".cc";
  std::string interface_file_name = file_name + ".h";
  if (!LineCount::fileExists(implementation_file_name) ||
      !LineCount::fileExists(interface_file_name)) {
    _is_good = false;
    return;
  }
  if ((!_is_good) || _loc != 0 || _method_count != 0) {
    _is_good = false;
    return;
  }
  std::ifstream interface_file(interface_file_name);
  for (std::string line; getline(interface_file, line);) {
    if (LineCount::lineIsComment(line)) continue;
    if (LineCount::lineHasSemicolon(line)) {
      ++_loc;
      ++_method_count;
    }
  }
  // Need to account the semicolon at the end of the class declaration.
  --_method_count;
  std::ifstream implementation_file(interface_file_name);
  for (std::string line; getline(implementation_file, line);) {
    if (LineCount::lineIsComment(line)) continue;
    if (LineCount::lineHasSemicolon(line)) ++_loc;
  }
  return;
}


/** Clears all internal variables.
 *
 * Subject to the same restrictions and expectations as LineCount::clear().
 */

void ObjectCount::clear() {
  _is_good = true;
  _loc = 0;
  _method_count = 0;
}


/** Returns the number of methods.
 *
 * This variable is subject to the same restrictions and expectations as LOC in
 * the LineCount class.
 */

unsigned ObjectCount::methods() {
  return _method_count;
}


/** Returns the part of the filename without the extension
 *
 * Searches for any occurence of a period, and if there is one, it returns the
 * part of the string up to but not including the period. Otherwise, it returns
 * the original string just as it was passed in.
 */

void ObjectCount::removeExtension(std::string *file_name) {
    size_t last_dot = file_name->find_last_of(".");
    if (last_dot == std::string::npos) return;
    *file_name = file_name->substr(0, last_dot);
}


}  // End namespace lc
