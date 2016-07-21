/**
 * Copyright 2014 Andrew Brinker
 */

#ifndef COURSE_H
#define COURSE_H

class Student;

#include <string>
#include <list>
#include "./Student.h"

class Course {
 public:
  Course(std::string, unsigned);
  std::string getName();
  std::list<Student *> getStudents();
  bool full();
  void addStudent(Student *);
 private:
  std::string name;
  unsigned max_enrollment;
  std::list<Student *> students;
};

#endif  // COURSE_H
