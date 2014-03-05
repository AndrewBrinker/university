/**
 * Copyright 2014 Andrew Brinker
 */

#include "./Scheduler.h"
#include <fstream>
#include <iostream>

Scheduler::Scheduler(std::ifstream &students_file,
                     std::ifstream &courses_file) {
    loadCourses(courses_file);
    loadStudents(students_file);
}

Scheduler::~Scheduler() {
    for (auto item : courses)  delete item;
    for (auto item : students) delete item;
}

void Scheduler::makeReports() {
    printf("Courses:\n");
    for (auto course : courses) {
        printf("\tName: %s\n", course->getName().c_str());
        printf("\tStudents:\n");
        for (auto student : course->getStudents()) {
            printf("\t\tName: %s\n", student->getName().c_str());
        }
    }
    printf("Students:\n");
    for (auto student : students) {
        printf("\tName: %s\n", student->getName().c_str());
        printf("\tCourses:\n");
        for (auto course : student->getCourses()) {
            printf("\t\tName: %s\n", course->getName().c_str());
        }
    }
}

void Scheduler::loadStudents(std::ifstream &students_file) {
    std::string name;
    std::string course;
    while (students_file >> name >> course) {
        Student *new_student = new Student(name);
        students.push_back(new_student);
        if (courseHasSpace(course)) {
            addStudentToCourse(course, new_student);
        }
    }
}

void Scheduler::loadCourses(std::ifstream &courses_file) {
    std::string name;
    int max_enrollment;
    while (courses_file >> name >> max_enrollment) {
        Course *new_course = new Course(name, max_enrollment);
        courses.push_back(new_course);
    }
}

bool Scheduler::courseHasSpace(std::string name) {
    for (auto course : courses) {
        if (course->getName() == name) {
            return !course->full();
        }
    }
    return false;
}

void Scheduler::addStudentToCourse(std::string name, Student *student) {
    for (auto course : courses) {
        if (course->getName() == name) {
            course->addStudent(student);
            student->addCourse(course);
        }
    }
}
