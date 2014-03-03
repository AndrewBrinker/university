/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <list>

#ifndef GRAMMARFILE_H
#define GRAMMARFILE_H

namespace gram {

class grammarfile {
 public:
  grammarfile();
  explicit grammarfile(std::list<std::string>);
  grammarfile(const grammarfile &);

  // Assignment operator
  grammarfile& operator=(const grammarfile &);

  // Get the next line and increment the sentry.
  bool getline(std::string *);
  void clear();

  // Variables
  std::list<std::string> contents;
  std::list<std::string>::iterator sentry;

 private:
  bool _is_clear;
};

}  // namespace gram

#endif  // GRAMMARFILE_H
