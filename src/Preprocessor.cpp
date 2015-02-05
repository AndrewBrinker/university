/**
 * Copyright 2015 Andrew Brinker
 */

#include "./Preprocessor.h"
#include <TransitionTable.h>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>
#include <sstream>

std::vector<std::string> split(const std::string str) {
  std::vector<std::string> words;

  std::istringstream iss(str);
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(),
            std::back_inserter(words));

  return words;
}

/**
 * Set internal variables and load the file
 * @param  file_name  -> The name of the file to be loaded
 */
Preprocessor::Preprocessor() {}


/**
 * Run the preprocessing and return the TransitionTable
 * @return the loaded TransitionTable
 */
TransitionTable Preprocessor::run(std::string file_name) {
  std::ifstream input_file(file_name);

  TransitionTable t;

  for (std::string line; getline(input_file, line);) {
    if (line.substr(0, 7) == "#start:") {
      // Add start state
      std::string start_state = line.substr(8, line.length() - 1);
      t.start_id = std::stoi(start_state);
      continue;
    }
    if (line.substr(0, 8) == "#accept:") {
      // Add accepting states
      std::string accept_states = line.substr(9, line.length() - 1);
      std::vector<std::string> words = split(accept_states);
      for (std::string word : words) {
        t.accept_id.push_back(std::stoi(word));
      }
      continue;
    }
    // Otherwise, process the line like normal by constructing a string stream
    std::istringstream iss(line);
    Transition ts;
    iss >> ts.src_id >> ts.dest_id >> ts.expr;
    t.transitions.push_back(ts);
  }

  return t;
}

