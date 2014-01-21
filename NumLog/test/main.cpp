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
   * Get the user's input.
   */
  std::cout << "Input numeric values. End with ^D." << std::endl;
  float input = 0.0;
  std::vector<float> values;
  std::cin >> input;
  do {
    values.push_back(input);
    std::cin >> input;
  } while (std::cin.good());
  logger.write(values);

  /*
   * Output what they input.
   */
  std::cout << "Here's what you input: " << std::endl;
  logger.read(&values);
  for (unsigned i = 0; i < values.size(); ++i) {
    std::cout << (float) values[i] << std::endl;
  }

  std::cout << "\nTests passed. Results are saved in " << filename << std::endl;
  return 0;
}
