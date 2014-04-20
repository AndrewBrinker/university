#include "./Person.h"
#include <string>

Person::Person(std::string new_name,
               std::string new_month,
               std::string new_day,
               std::string new_year) {
  _name  = new_name;
  _month = new_month;
  _day   = new_day;
  _year  = new_year;
}

std::string Person::name() {
  return _name;
}

std::string Person::month() {
  return _month;
}

std::string Person::day() {
  return _day;
}

std::string Person::year() {
  return _year;
}