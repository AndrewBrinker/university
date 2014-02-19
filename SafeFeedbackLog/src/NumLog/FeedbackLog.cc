/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 * Assignment: 3B
 *
 * Copyright 2014 Andrew Brinker
 */

#include "./FeedbackLog.h"
#include <sys/stat.h>
#include <list>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>


#define FL_MSG_IN  0
#define FL_MSG_OUT 1


FeedbackLog::FeedbackLog(std::string name) : file_name(name) {
  input();
  process();
  output();
}


void FeedbackLog::input() {
  std::ifstream file_stream(file_name);
  for (std::string line; std::getline(file_stream, line);) {
    data.push_back(atof(line.c_str()));
  }
  file_stream.close();
}


void FeedbackLog::output() {
  std::cout << "Save to file: ";
  std::cin >> file_name;
  std::ofstream file_stream(file_name, std::ios::out);
  for (auto it = data.begin(); it != data.end(); ++it) {
    file_stream << std::fixed << std::setprecision(5) << *it << "\n";
  }
  file_stream.close();
}

void FeedbackLog::process() {
  message(FL_MSG_IN);
  std::string input;
  float new_num;
  for (auto it = data.begin(); it != data.end(); ++it) {
    std::cout << "The current number is: " << *it << "\n"
              << "\tPick an action: ";
    std::cin >> input;
    switch (input[0]) {
      case 'A':
      case 'a':
        break;
      case 'R':
      case 'r':
        std::cin.clear();
        std::cin.ignore();
        while (true) {
          std::cout << "\tPick a new number: ";
          std::getline(std::cin, input);
          std::stringstream stream(input);
          if (stream >> new_num) break;
          std::cout << "Not a number. Please try again." << std::endl;
        }
        *it = new_num;
        break;
      case 'D':
      case 'd':
        it = data.erase(it);
        --it;
        break;
      case 'I':
      case 'i':
        std::cin.clear();
        std::cin.ignore();
        while (true) {
          std::cout << "\tInsert a number: ";
          std::getline(std::cin, input);
          std::stringstream stream(input);
          if (stream >> new_num) break;
          std::cout << "Not a number. Please try again." << std::endl;
        }
        ++it;
        it = data.insert(it, new_num);
        break;
      case 'Q':
      case 'q':
        it = data.end();
        --it;
        break;
      default:
        std::cout << "Invalid input. Please try again." << std::endl;
        --it;
    }
  }
  message(FL_MSG_OUT);
}


void FeedbackLog::message(const unsigned code) const {
  if (code == FL_MSG_IN) {
    std::cout << "Welcome to your interactive logging session.\n"
              << "\n"
              << "For each number you may:\n"
              << "\t1) [A]ccept\n"
              << "\t2) [R]eplace\n"
              << "\t3) [D]elete\n"
              << "\t4) [I]nsert\n"
              << "\t5) [Q]uit & Save\n"
              << "\n";
  } else if (code == FL_MSG_OUT) {
    std::cout << "Finished. Exiting." << std::endl;
  }
}


bool FeedbackLog::fileExists(std::string file_name) {
  struct stat buffer;
  return (stat(file_name.c_str(), &buffer) == 0);
}

