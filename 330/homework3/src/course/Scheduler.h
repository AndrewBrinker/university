/**
 * Copyright 2014 Andrew Brinker
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
#include <list>
#include <fstream>
#include "./Student.h"
#include "./Scheduler.h"

class Scheduler {
 public:
  Scheduler(std::ifstream &, std::ifstream &);
  ~Scheduler();
  void makeReports();
 private:
  void loadStudents(std::ifstream &);
  void loadCourses(std::ifstream &);

  bool courseHasSpace(std::string);
  void addStudentToCourse(std::string, Student *);

  std::list<Student *> students;
  std::list<Course *> courses;
};

#endif  // SCHEDULER_H
