/**
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <list>

#ifndef GRAMMARFILE_H
#define GRAMMARFILE_H

namespace gram {

class GrammarFile {
 public:
  GrammarFile();
  explicit GrammarFile(std::list<std::string>);
  GrammarFile(const GrammarFile &);

  // Assignment operator
  GrammarFile& operator=(const GrammarFile &);

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
