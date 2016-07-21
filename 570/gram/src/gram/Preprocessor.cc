/**
 * Copyright 2014 Andrew Brinker
 */

#include "./Preprocessor.h"
#include <string>
#include <set>
#include <fstream>
#include <cstdio>
#include "./Grammar.h"

#define COMMENT   "#"
#define SPLIT     "|"
#define SEP       "->"
#define DELIM     "$"


/**
 * Set internal variables and load the file
 * @param  file_name  -> The name of the file to be loaded
 */
Preprocessor::Preprocessor(std::string file_name)
                         : name(file_name),
                           _is_expanded(false) {
    file = load();
}


/**
 * Run the preprocessing and return the Grammar
 * @return the loaded Grammar
 */
Grammar Preprocessor::run() {
    if (_is_expanded) return file;
    return expand();
}


/**
 * Load the file into a Grammar, and set internal variables appropriately
 * @return the loaded Grammar
 */
Grammar Preprocessor::load() {
    std::ifstream input_file(name);
    std::list<std::string> intermediary;
    for (std::string line; getline(input_file, line);) {
        if (line[0] == COMMENT[0]) continue;
        if (line == "\n") continue;
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        intermediary.push_back(line + "\n");
    }
    input_file.close();
    std::string first_line = *(intermediary.begin());
    if (!(first_line.find(SEP) != std::string::npos)) _is_expanded = true;
    Grammar g(intermediary);
    return g;
}


/**
 * Expand the given Grammar for reading by the parser
 * @return the expanded Grammar
 */
Grammar Preprocessor::expand() {
  std::set<std::string> terminals;
  for (auto it = file.contents.begin(); it != file.contents.end(); ++it) {
    size_t i = 0;
    std::string lhs = it->substr(0, 1);
    std::string rhs = it->substr(3);
    while (i < rhs.length()) {
      if (rhs[i] == SPLIT[0]) {
        *it = it->substr(0, i + 3) + "\n";
        file.contents.push_back(lhs + SEP + rhs.substr(i+1));
        break;
      } else if (!isupper(rhs[i]) && rhs[i] != '\n') {
        terminals.insert(rhs.substr(i, 1));
      }
      ++i;
    }
  }
  std::string delim_line = std::string(DELIM) + "\n";
  file.contents.push_front(delim_line);
  for (auto symbol : terminals) file.contents.push_front(symbol + "\n");
  file.contents.push_back(delim_line);
  sanityCheck();
  return file;
}


/**
 * Verify that the file is a valid grammar file
 */
void sanityCheck() {
  /*
   * 1) No uppercase in non-terminals
   * 2) Two section delimiters
   * 3) Each production line has an arrow
   * 4) No whitespace
   */
}
