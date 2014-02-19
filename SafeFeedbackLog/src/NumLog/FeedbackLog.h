/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 * Assignment: 3B
 *
 * Copyright 2014 Andrew Brinker
 */

#include <string>
#include <list>

#ifndef FEEDBACKLOG_H
#define FEEDBACKLOG_H

class FeedbackLog {
 public:
  explicit FeedbackLog(std::string);

 private:
  void input();
  void output();
  void process();
  void message(const unsigned) const;

  bool fileExists(std::string);

  std::string file_name;
  std::list<float> data;
};

#endif  // FEEDBACKLOG_H
