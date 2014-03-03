/**
 * Copyright 2014 Andrew Brinker
 */

#include "./preprocessor.h"
#include <string>
#include <set>
#include <fstream>
#include <cstdio>
#include "./grammarfile.h"

#define COMMENT   "#"
#define SPLIT     "|"
#define SEP       "->"
#define DELIM     "$"

namespace gram {

preprocessor::preprocessor(std::string file_name)
                         : name(file_name),
                           _is_expanded(false) {
    file = load();
}

grammarfile preprocessor::run() {
    if (_is_expanded) return file;
    return expand();
}

grammarfile preprocessor::load() {
    // Strip out whitespace
    // Strip out comments
    // If the first line is a terminal, set _is_expanded to true
    // Return the stripped grammarfile
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
    grammarfile g(intermediary);
    return g;
}

grammarfile preprocessor::expand() {
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
  return file;
}

}  // namespace gram
