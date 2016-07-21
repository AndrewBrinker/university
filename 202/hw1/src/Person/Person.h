#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
 public:
  Person(std::string, std::string, std::string, std::string);
  std::string name();
  std::string month();
  std::string day();
  std::string year();

 private:
  std::string _name;
  std::string _month;
  std::string _day;
  std::string _year;
};

#endif  // PERSON_H
