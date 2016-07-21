/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 *
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <list>

#ifndef NUMBERLOG_H
#define NUMBERLOG_H

class NumberLog {
 public:
  explicit NumberLog(std::string);
  inline std::list<double> getData() { return data; }

 private:
  void input();
  bool fileExists(std::string);

  std::string file_name;
  std::list<double> data;
};

#endif  // NUMBERLOG_H
