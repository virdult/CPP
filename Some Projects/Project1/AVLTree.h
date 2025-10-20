#ifndef AVLTREE_H
#define AVLTREE_H

#include "Student.h"
#include <memory>
#include <fstream>

class AVLTree {
private:
    struct Node {
        double gpa;
        std::vector<Student> students; // multiple students with same GPA
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        int height;

        Node(double g, const Student& s)
            : gpa(g), height(1) {
            students.push_back(s);
        }
    };

    std::unique_ptr<Node> root;

    // AVL utilities
    int getHeight(const std::unique_ptr<Node>& n) const;
    int getBalance(const std::unique_ptr<Node>& n) const;
    void updateHeight(Node* n);
    std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> y);
    std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> x);
    std::unique_ptr<Node> insertNode(std::unique_ptr<Node> node, const Student& s);
    std::unique_ptr<Node> deleteById(std::unique_ptr<Node> node, int id);
    Node* minValueNode(Node* node);
    void inorder(const Node* node) const;
    void saveToFile(const Node* node, std::ofstream& file) const;
    bool idExists(const Node* node, int id) const;
    const Student* findById(const Node* node, int id) const;

public:
    AVLTree() = default;

    void insert(const Student& s);
    void displayAll() const;
    void deleteStudentById(int id);
    void save(const std::string& filename) const;
    void load(const std::string& filename);
    const Student* findStudentById(int id) const;
};

#endif
