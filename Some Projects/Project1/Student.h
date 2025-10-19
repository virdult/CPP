#ifndef STUDENT_H
#define STUDENT_H

#include <string>

class Student {
public:
    int id;
    double gpa;
    std::string name;

    Student(int id, double gpa, const std::string& name);
};

#endif
