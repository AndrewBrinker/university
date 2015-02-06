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


/*=============================================================================
 * Structs
 **/

/**
 * @brief   An individual transition in a transition table.
 * @details Indicates a single arrow in a Finite Automata (of any sort). Has a
 *          src_id, a dest_id, and an expression. Can be used for a DFA, NFA,
 *          or GNFA.
 */
struct transition {
  int    src_id;
  int    dest_id;
  string expr;
};

/**
 * @brief   A transition table, completely characterizing an FA.
 * @details A complete table of transitions, representing a Finite Automata. Has
 *          a start_id, a vector of accepting ids, and a vector of individual
 *          transitions.
 */
struct transition_table {
  int                start_id;
  vector<int>        accept_id;
  vector<transition> transitions;
};


/*=============================================================================
 * Private Functions
 **/

/**
 * @brief   Split a string on spaces.
 * @details Split a string into a vector of strings, splitting on spaces.
 *
 * @param   str - The string to be split
 * @return  the vector of split-up substrings, in order of appearance in str.
 */
static vector<string> split(const string str) {
  vector<string> words;

  istringstream iss(str);
  copy(istream_iterator<string>(iss),
            istream_iterator<string>(),
            back_inserter(words));

  return words;
}

/**
 * @brief   Check whether a string starts with the given substring.
 * @details Check whether a string starts with the given substring. Will return
 *          false if sub is longer than str.
 *
 * @param   str - The string whose substring is being checked.
 * @param   sub - The substring being checked against
 *
 * @return  whether str starts with sub.
 */
static bool starts_with(const string &str, const string &sub) {
  if (sub.length() > str.length()) {
    return false;
  }
  return str.substr(0, sub.length() - 1) == sub;
}

/**
 * @brief   Get the contents of a string from idx to end.
 * @details Get the contents of a string from idx to end. Returns an empty
 *          string if the idx is too big.
 *
 * @param   str - The string being sliced
 * @param   idx - The index to start at
 *
 * @return  String slice from idx to end.
 */
static string slice_from(string str, size_t idx) {
  if (idx > str.length() - 1) return "";
  return string(str.begin() + idx, str.end() - 1);
}


/*=============================================================================
 * Public Functions
 **/

/**
 * @brief   Load a transition table from its file representation.
 * @details Load a transition table from the file with the given name. This
 *          assumes that the file is the correct format, and will fail if it
 *          isn't.
 *
 * @param   name - The name of the file to load
 * @return  The transition table constructed from the file.
 */
transition_table load(string name) {
  transition_table table;
  ifstream input(name);

  // For each line in the file...
  for (string line; getline(input, line);) {

    // Set start state.
    if (starts_with(line, "#start:")) {
      table.start_id = stoi(slice_from(line, 8));
    }

    // Set accept states.
    if (starts_with(line, "#accept:")) {
      auto states = split(slice_from(line, 9));
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

  return table;
}
