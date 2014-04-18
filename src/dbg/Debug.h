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
  Debug() {
    _debug = false;
    source = std::vector<std::string>();
  }

  inline void activate() {
    _debug = true;
  }

  inline void deactivate() {
    _debug = false;
  }

  inline bool on() {
    return _debug;
  }

  std::vector<std::string> source;

 private:
  bool _debug;

  DISALLOW_COPY_AND_ASSIGN(Debug);
};

#endif  // DEBUG_H
