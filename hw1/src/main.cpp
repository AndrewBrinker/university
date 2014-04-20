#include <Person/Person.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

int main() {
  std::string file_name = "";
  std::cout << "File name: ";
  std::cin >> file_name;
  std::ifstream input_file(file_name);
  if (!input_file.is_open()) {
    std::cout << "File opening failed." << std::endl;
    return 0;
  }
  std::vector<Person> friends;
  while (input_file.good()) {
    std::string name  = "";
    std::string month = "";
    std::string day   = "";
    std::string year  = "";
    input_file >> name >> month >> day >> year;
    friends.push_back(Person(name, month, day, year));
  }

  input_file.close();

  std::string month = "";
  std::cout << "Month: ";
  std::cin >> month;

  for (Person person : friends) {
    if (person.month() == month) {
      std::cout << person.name()  << " "
                << person.month() << " "
                << person.day()   << " "
                << person.year()  << " "
                << std::endl;
    }
  }
  return 0;
}