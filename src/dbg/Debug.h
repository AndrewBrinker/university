/**
 * Copyright 2014 Andrew Brinker, Anthony Sterrett
 */

#ifndef DEBUG_H
#define DEBUG_H

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&);

class Debug {
 public:
  static Debug& Instance() {
    static Debug instance;
    return instance;
  }

  bool on;
  std::vector<std::string> source;

 private:
  Debug() {
    on = false;
    source = std::vector<std::string>();
  }
  DISALLOW_COPY_AND_ASSIGN(Debug);
};

#endif  // DEBUG_H
