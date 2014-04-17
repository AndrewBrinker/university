/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef CLI_H
#define CLI_H

#include <map>
#include <vector>
#include <string>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class CLI {
 public:
  CLI(const int, char **);
  void parse();

 private:
  std::map<std::string, bool> args;
  std::vector<std::string> files;

  void printHelp();
  void printVersion();
  void startInteractiveSession();

  DISALLOW_COPY_AND_ASSIGN(CLI);
};

#endif  // CLI_H
