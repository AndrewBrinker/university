/**
 * Copyright 2015 Andrew Brinker
 */

#include <transition_table.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;



/*=============================================================================
 * Private Functions
 **/


/**
 * @brief   Calculate the length of a C-string at compile time.
 * @details Calculates the length of a C-string at compile time by essentially
 *          emulating a left fold operation on the string, terminating at null.
 *
 * @param   str - The string whose length is being calculated
 * @return  The length of the string.
 */
static constexpr int c_len(const char *str) {
    return *str ? 1 + c_len(str + 1) : 0;
}


/**
 * @brief   Calculate the length of a C++ string at compile time.
 * @details Simply calls c_len() on the C string inside str.
 *
 * @param   str - The string whose length is being calculated
 * @return  The length of the string.
 */
static constexpr int len(const string &str) {
  return c_len(str.c_str());
}


/**
 * @brief   Split a string on spaces.
 * @details Split a string into a vector of strings, splitting on spaces.
 *
 * @param   str - The string to be split
 * @return  the vector of split-up substrings, in order of appearance in str.
 */
static vector<string> split(const string &str) {
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
  if (sub.length() > str.length()) return false;
  return str.substr(0, sub.length()) == sub;
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
static string slice_from(const string &str, const size_t &idx) {
  if (idx > str.length() - 1) return "";
  return string(str.begin() + idx, str.end());
}



/*=============================================================================
 * Public Functions
 **/


/**
 * @brief   Crash the program with the given message.
 * @details Prints out the given message and then exits with a failure.
 *
 * @param   msg - The message to be printed
 */
void crash(const std::string &msg) {
  printf("%s\n", msg.c_str());
  exit(EXIT_FAILURE);
}


/**
 * @brief   Load a transition table from its file representation.
 * @details Load a transition table from the file with the given name. This
 *          assumes that the file is the correct format, and will fail if it
 *          isn't.
 *
 * @param   name - The name of the file to load
 * @return  The transition table constructed from the file.
 */
transition_table load(const string &name) {
  transition_table table;
  ifstream input(name);
  string line;

  const string start_str  = "#start: ";
  const string accept_str = "#accept: ";

  // Set start state.
  getline(input, line);
  if (!starts_with(line, start_str)) {
    crash("Missing start state on line 1.");
  }
  table.start_id = stoi(slice_from(line, len(start_str)));

  // Set accept states.
  getline(input, line);
  if (!starts_with(line, accept_str)) {
    crash("Missing accepting states on line 2.");
  }
  vector<string> states = split(slice_from(line, len(accept_str)));
  table.accept_ids.resize(states.size());
  transform(states.begin(), states.end(), table.accept_ids.begin(),
            [] (string &s) -> int { return stoi(s); });

  // For each line in the file add a new transition to the table
  while (getline(input, line)) {
    transition row;
    istringstream iss(line);
    iss >> row.src_id >> row.dest_id >> row.expr;
    table.transitions.push_back(row);
  }

  return table;
}


/**
 * @brief   Display an individual FA transition.
 * @details Pretty prints an individual transition in an FA, indicating the
 *          source state, the destination state, and the transition expression.
 *
 * @param t - The transition being displayed
 */
void show_transition(const transition &t) {
  printf("%d -> %d :: %s\n", t.src_id, t.dest_id, t.expr.c_str());
}


/**
 * @brief   Display an FA
 * @details Pretty prints a finite automata, indicating the start state,
 *          accepting states, and transitions.
 *
 * @param table - The transition table being displayed
 */
void show_table(const transition_table &table) {
  printf("Start:\t%d\n", table.start_id);

  printf("Accept:\t");
  for (int id : table.accept_ids) {
    printf("%d ", id);
  }
  printf("\n");

  printf("Transitions:\n");
  for (transition t : table.transitions) {
    printf("\t");
    show_transition(t);
  }
}


