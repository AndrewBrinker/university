/**
 * Copyright 2015 Andrew Brinker
 */

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;


struct transition {
  int    src_id;
  int    dest_id;
  string expr;
};

struct transition_table {
  int                start_id;
  vector<transition> transitions;
  vector<int>        accept_id;
};


static vector<string> split(const string str) {
  vector<string> words;

  istringstream iss(str);
  copy(istream_iterator<string>(iss),
            istream_iterator<string>(),
            back_inserter(words));

  return words;
}

static bool starts_with(const string &str, const string &partial) {
  return str.substr(0, partial.length() - 1) == partial;
}

static string slice_to_end(string::iterator begin, string::iterator end) {
  return string(begin, end - 1);
}

transition_table load(string name) {
  transition_table table;
  ifstream input(name);

  // For each line in the file...
  for (string line; getline(input, line);) {

    // Set start state.
    if (starts_with(line, "#start:")) {
      table.start_id = stoi(slice_to_end(line.begin() + 8, line.end()));
    }

    // Set accept states.
    if (starts_with(line, "#accept:")) {
      auto states = split(slice_to_end(line.begin() + 9, line.end()));
      for (string state : states) {
        table.accept_id.push_back(stoi(state));
      }
    }

    // Otherwise, add a new transition to the transition table.
    else {
      transition row;
      istringstream iss(line);
      iss >> row.src_id >> row.dest_id >> row.expr;
      table.transitions.push_back(row);
    }
  }

  // Return the transition table.
  return table;
}
