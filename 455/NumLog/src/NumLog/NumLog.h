/*
 * Copyright 2014 Andrew Brinker.
 */

#include <string>
#include <vector>

#ifndef NUMLOG_H
#define NUMLOG_H

class NumLog {
 public:
  explicit NumLog(std::string);
  int write(std::vector<float>);
  int read(std::vector<float> *);

 private:
  std::string file_name;
};

#endif  // NUMLOG_H
