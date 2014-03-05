/**
 * Copyright 2014 Andrew Brinker
 */

#include <course/Scheduler.h>
#include <cstdio>
#include <fstream>

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: ./test <students file> <courses file>\n");
        return 0;
    }

    std::ifstream students_file(argv[1]);
    std::ifstream courses_file(argv[2]);

    Scheduler s(students_file, courses_file);
    s.makeReports();

    students_file.close();
    courses_file.close();

    return 0;
}
