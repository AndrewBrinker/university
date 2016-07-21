/**
 * Copyright 2014 Andrew Brinker
 */

#include "./Course.h"
#include <string>

Course::Course(std::string new_name, unsigned new_max) {
    name = new_name;
    max_enrollment = new_max;
}

std::string Course::getName() {
    return name;
}

std::list<Student *> Course::getStudents() {
    return students;
}

bool Course::full() {
    return students.size() >= max_enrollment;
}

void Course::addStudent(Student *s) {
    students.push_back(s);
}
