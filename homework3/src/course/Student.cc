/**
 * Copyright 2014 Andrew Brinker
 */

#include "./Student.h"
#include <list>
#include <string>
#include "./Course.h"

Student::Student(std::string new_name) : name(new_name) {}

std::string Student::getName() {
    return name;
}

std::list<Course *> Student::getCourses() {
    return courses;
}

void Student::addCourse(Course *c) {
    courses.push_back(c);
}

void Student::removeCourse(std::list<Course *>::iterator &c) {
    courses.erase(c);
}

std::list<Course *>::iterator Student::firstCourse() {
    return courses.begin();
}

std::list<Course *>::iterator Student::lastCourse() {
    return courses.end();
}
