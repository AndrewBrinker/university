/**
 * Copyright 2014 Andrew Brinker
 */

#ifndef STUDENT_H
#define STUDENT_H

class Course;

#include <list>
#include <string>
#include "./Course.h"

class Student {
 public:
  explicit Student(std::string);
  std::string getName();
  std::list<Course *> getCourses();
  void addCourse(Course *);
  void removeCourse(std::list<Course *>::iterator &);
  std::list<Course *>::iterator firstCourse();
  std::list<Course *>::iterator lastCourse();
 private:
  std::string name;
  std::list<Course *> courses;
};

#endif  // STUDENT_H
