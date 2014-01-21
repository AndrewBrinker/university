/*
 * Copyright 2014 Andrew Brinker.
 */

#include <NumLog/NumLog.h>
#include <string>
#include <vector>
#include <iostream>

int main() {
  /*
   * Choose what file to use.
   */
  std::cout << "Choose a filename: ";
  std::string filename = "";
  std::cin >> filename;
  NumLog logger(filename);

  /*
   * Choose a mode (read or write).
   */
  std::cout << "Choose mode ('read' or 'write'): ";
  std::string mode = "";
  std::cin >> mode;

  if (mode == "write") {
    /*
     * Get the user's input.
     */
    std::cout << "\tInput numeric values. End with ^D." << std::endl;
    float input = 0.0;
    std::vector<float> values;
    std::cin >> input;
    do {
      values.push_back(input);
      std::cin >> input;
    } while (std::cin.good());
    logger.write(values);
    std::cout << "\n\tResults are saved in "
              << filename << std::endl;
  } else if (mode == "read") {
    /*
     * Output what they input.
     */
    std::vector<float> values;
    std::cout << "\tHere are the numbers stored in : "
              << filename << std::endl;
    logger.read(&values);
    for (unsigned i = 0; i < values.size(); ++i) {
      std::cout << (float) values[i] << std::endl;
    }
  } else {
    std::cout << "\tMode not recognized. Please try again." << std::endl;
  }

  return 0;
}
