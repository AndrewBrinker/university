/**
 * Copyright 2014 Andrew Brinker
 */

#include "./preprocessor.h"
#include <string>
#include <cstdio>
#include "./grammarfile.h"

namespace gram {

preprocessor::preprocessor(std::string file_name) {
    printf("%s", file_name.c_str());
}

grammarfile preprocessor::run() {
    grammarfile g;
    return g;
}

/*
void parser::expandFile(std::string file_name) {
  // 1) Ignore blank lines (done)
  // 2) Ignore comments (done)
  // 3) Remove whitespace (done)
  // 4) Split across OR (done)
  // 5) Populate list of terminals
  // 6) Insert section delimiters (done)
  std::ifstream input_file(file_name);
  std::list<std::string> intermediary;
  std::set<std::string> terminals;
  for (std::string line; getline(input_file, line);) {
    if (line[0] == COMMENT[0]) continue;
    if (line == "\n") continue;
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    intermediary.push_back(line + "\n");
  }
  input_file.close();
  for (auto it = intermediary.begin(); it != intermediary.end(); ++it) {
    if (it->size() <= 3) {
      _condensed = false;
      break;
    }
    size_t i = 0;
    std::string lhs = it->substr(0, 1);
    std::string rhs = it->substr(3);
    while (i < rhs.length()) {
      if (rhs[i] == SPLIT[0]) {
        *it = it->substr(0, i + 3) + "\n";
        intermediary.push_back(lhs + "->" + rhs.substr(i+1));
        break;
      } else if (!isupper(rhs[i]) && rhs[i] != '\n') {
        terminals.insert(rhs.substr(i, 1));
      }
      ++i;
    }
  }
  if (_condensed) {
    std::string delim_line = DELIM;
    delim_line += "\n";
    intermediary.push_front(delim_line);
    for (auto symbol : terminals) {
      intermediary.push_front(symbol + "\n");
    }
    intermediary.push_back(delim_line);
    std::ofstream output_file(TEMP_FILE);
    for (auto line : intermediary) {
      output_file << line;
    }
    output_file.close();
  }
}
*/

}  // namespace gram
