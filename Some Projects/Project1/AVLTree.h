#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <string>
#include "Student.h"

class AVLNode {
public:
    double gpa;
    std::vector<std::shared_ptr<Student>> students;
    int height;
    std::shared_ptr<AVLNode> left;
    std::shared_ptr<AVLNode> right;

    explicit AVLNode(std::shared_ptr<Student> s);
};

class AVLTree {
private:
    std::shared_ptr<AVLNode> root;

    int getHeight(const std::shared_ptr<AVLNode>& n) const;
    int getBalance(const std::shared_ptr<AVLNode>& n) const;
    std::shared_ptr<AVLNode> rightRotate(std::shared_ptr<AVLNode> y);
    std::shared_ptr<AVLNode> leftRotate(std::shared_ptr<AVLNode> x);
    static bool lessThan(const std::shared_ptr<Student>& a, const std::shared_ptr<Student>& b);

    std::shared_ptr<AVLNode> insertNode(std::shared_ptr<AVLNode> node, std::shared_ptr<Student> s);
    std::shared_ptr<AVLNode> findMin(std::shared_ptr<AVLNode> node);
    std::shared_ptr<AVLNode> deleteNodeByGPA(std::shared_ptr<AVLNode> root, double gpa);
    std::shared_ptr<AVLNode> deleteStudentRec(std::shared_ptr<AVLNode> root, int id);

    void inorder(const std::shared_ptr<AVLNode>& node) const;
    void saveRec(std::ofstream& out, const std::shared_ptr<AVLNode>& node) const;

public:
    AVLTree();
    void insert(std::shared_ptr<Student> s);
    void deleteStudent(int id);
    void display() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

#endif
