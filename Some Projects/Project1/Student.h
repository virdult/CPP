#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <iostream>

class Student {
public:
    int id;
    std::string name;
    double gpa;

    Student(int id, const std::string& name, double gpa); // only declare

    void display() const;
};

#endif
