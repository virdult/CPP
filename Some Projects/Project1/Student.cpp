#include "Student.h"
#include <iostream>

Student::Student(int id, const std::string& name, double gpa)
    : id(id), name(name), gpa(gpa) {}

void Student::display() const {
    std::cout << "ID: " << id << " | Name: " << name << " | GPA: " << gpa << '\n';
}
