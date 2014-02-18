/**
 * Name:       Andrew Brinker
 * Class:      CSE 455, Software Engineering (Winter 2014)
 * School:     California State University, San Bernardino
 * Assignment: 2B
 *
 * Copyright 2014 Andrew Brinker
 */

#include "./InteractiveLog.h"
#include <list>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>

typedef std::list<float>::iterator list_iterator;


InteractiveLog::InteractiveLog(std::string name) : file_name(name) {
  read();
  run();
  save();
}


void InteractiveLog::run() {
  displayIntroMessage();
  std::string input;
  float new_num;
  for (list_iterator it = data.begin(); it != data.end(); ++it) {
    start:
    std::cout << "The current number is: " << *it << "\n"
              << "\tPick an action: ";
    std::cin >> input;
    switch (input[0]) {
      case 'A': case 'a':
        break;
      case 'R': case 'r':
        std::cout << "\tPick a new number: ";
        std::cin >> new_num;
        *it = new_num;
        break;
      case 'D': case 'd':
        it = data.erase(it);
        --it;
        break;
      case 'I': case 'i':
        std::cout << "\tInsert a number: ";
        std::cin >> new_num;
        ++it;
        it = data.insert(it, new_num);
        break;
      case 'Q': case 'q':
        goto end;
      default:
        std::cout << "Invalid input. Please try again." << std::endl;
        goto start;
    }
  }
  end:
  displayExitMessage();
}


void InteractiveLog::read() {
  std::ifstream file_stream(file_name);
  for (std::string line; std::getline(file_stream, line);) {
    data.push_back(atof(line.c_str()));
  }
  file_stream.close();
}


void InteractiveLog::write() {
  std::ofstream file_stream(file_name, std::ios::out);
  for (list_iterator it = data.begin(); it != data.end(); ++it) {
    file_stream << std::fixed << std::setprecision(5) << *it << "\n";
  }
  file_stream.close();
}


void InteractiveLog::save() {
  std::cout << "Save to file: ";
  std::cin >> file_name;
  write();
}


void InteractiveLog::displayIntroMessage() {
  std::cout << "Welcome to your interactive logging session.\n"
            << "\n"
            << "For each number you may:\n"
            << "\t1) [A]ccept\n"
            << "\t2) [R]eplace\n"
            << "\t3) [D]elete\n"
            << "\t4) [I]nsert\n"
            << "\t5) [Q]uit & Save\n"
            << "\n";
}


void InteractiveLog::displayExitMessage() {
  std::cout << "Finished. Exiting." << std::endl;
}

