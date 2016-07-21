/*
 * Copyright 2014 Andrew Brinker.
 */

#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "./NumLog.h"


/** Create a new NumLog with the given filename.
 *
 * Assumes the file doesn't already exist, and makes a new one. Will delete any
 * existing file with the given name. It is up to the user to make sure that the
 * given file doesn't already exist.
 *
 * @param: std::string name
 *   - The name of the file being used.
 */
NumLog::NumLog(std::string name) {
  file_name = name;
}


/** Writes contents of vector to file.
 *
 * Numbers are written out one-per-line. Always appends to the given file. Will
 * never delete or overwrite existing content.
 *
 * @param: std::vector<float> input
 *   - The list of numbers to be written
 *
 * @return: EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int NumLog::write(std::vector<float> input) {
  std::ofstream file_stream(file_name, std::ios::out);
  for (size_t i = 0; i < input.size(); ++i) {
    file_stream << std::fixed << std::setprecision(5) << input[i] << "\n";
  }
  file_stream.close();
  return EXIT_SUCCESS;
}


/** Reads out contents from file.
 *
 * Reads contents from the file and add output them in a vector of floats.
 *
 * @param: std::vector<float> *output
 *   - A pointer to a vector which the file contents are written out to.
 *
 * @return: EXIT_SUCCESS if successful, EXIT_FAILURE otherwise.
 */
int NumLog::read(std::vector<float> *output) {
  std::ifstream file_stream(file_name);
  if (output->size() != 0) {
    file_stream.close();
    return EXIT_FAILURE;
  }
  for (std::string line; std::getline(file_stream, line);) {
    std::istringstream line_stream(line);
    float current;
    line_stream >> current;
    output->push_back(current);
  }
  file_stream.close();
  return EXIT_SUCCESS;
}
