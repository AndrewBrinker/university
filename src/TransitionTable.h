/**
 * Copyright 2015 Andrew Brinker
 */

#include <string>
#include <vector>

#ifndef TRANSITION_TABLE_H
#define TRANSITION_TABLE_H

class Transition {
 public:
  int src_id;
  int dest_id;
  std::string expr;
};

class TransitionTable {
 public:
  TransitionTable();
  TransitionTable(const TransitionTable&);
  const TransitionTable& operator=(const TransitionTable&);

  std::vector<Transition> transitions;
  int start_id;
  std::vector<int> accept_id;
};

TransitionTable::TransitionTable(const TransitionTable& other) {
  transitions = other.transitions;
  start_id = other.start_id;
  accept_id = other.accept_id;
}

const TransitionTable& TransitionTable::operator=(const TransitionTable& other) {
  transitions = other.transitions;
  start_id = other.start_id;
  accept_id = other.accept_id;
  return *this;
}

#endif  // TRANSITION_TABLE_H
