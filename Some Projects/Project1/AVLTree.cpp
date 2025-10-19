#include "AVLTree.h"
#include <algorithm>
#include <limits>

// ========== AVLNode Constructor ==========
AVLNode::AVLNode(std::shared_ptr<Student> s)
    : gpa(s->gpa), height(1), left(nullptr), right(nullptr) {
    students.push_back(s);
}

// ========== AVLTree Implementation ==========
AVLTree::AVLTree() : root(nullptr) {}

int AVLTree::getHeight(const std::shared_ptr<AVLNode>& n) const {
    return n ? n->height : 0;
}

int AVLTree::getBalance(const std::shared_ptr<AVLNode>& n) const {
    return n ? getHeight(n->left) - getHeight(n->right) : 0;
}

std::shared_ptr<AVLNode> AVLTree::rightRotate(std::shared_ptr<AVLNode> y) {
    auto x = y->left;
    auto T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

std::shared_ptr<AVLNode> AVLTree::leftRotate(std::shared_ptr<AVLNode> x) {
    auto y = x->right;
    auto T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

bool AVLTree::lessThan(const std::shared_ptr<Student>& a, const std::shared_ptr<Student>& b) {
    if (a->gpa == b->gpa)
        return a->id < b->id;
    return a->gpa < b->gpa;
}

std::shared_ptr<AVLNode> AVLTree::insertNode(std::shared_ptr<AVLNode> node, std::shared_ptr<Student> s) {
    if (!node)
        return std::make_shared<AVLNode>(s);

    if (s->gpa < node->gpa)
        node->left = insertNode(node->left, s);
    else if (s->gpa > node->gpa)
        node->right = insertNode(node->right, s);
    else {
        auto& v = node->students;
        auto it = v.begin();
        while (it != v.end() && (*it)->id < s->id) ++it;
        v.insert(it, s);
        return node;
    }

    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    int balance = getBalance(node);
    if (balance > 1 && s->gpa < node->left->gpa)
        return rightRotate(node);
    if (balance < -1 && s->gpa > node->right->gpa)
        return leftRotate(node);
    if (balance > 1 && s->gpa > node->left->gpa) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && s->gpa < node->right->gpa) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

std::shared_ptr<AVLNode> AVLTree::findMin(std::shared_ptr<AVLNode> node) {
    auto current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

std::shared_ptr<AVLNode> AVLTree::deleteStudentRec(std::shared_ptr<AVLNode> root, int id) {
    if (!root) return nullptr;

    auto& vec = root->students;
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if ((*it)->id == id) {
            vec.erase(it);
            break;
        }
    }

    if (vec.empty()) {
        if (!root->left) return root->right;
        if (!root->right) return root->left;
        auto temp = findMin(root->right);
        root->students = temp->students;
        root->gpa = temp->gpa;
        root->right = deleteNodeByGPA(root->right, temp->gpa);
    }

    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

std::shared_ptr<AVLNode> AVLTree::deleteNodeByGPA(std::shared_ptr<AVLNode> root, double gpa) {
    if (!root) return nullptr;
    if (gpa < root->gpa)
        root->left = deleteNodeByGPA(root->left, gpa);
    else if (gpa > root->gpa)
        root->right = deleteNodeByGPA(root->right, gpa);
    else {
        if (!root->left) return root->right;
        if (!root->right) return root->left;
        auto temp = findMin(root->right);
        root->students = temp->students;
        root->gpa = temp->gpa;
        root->right = deleteNodeByGPA(root->right, temp->gpa);
    }
    return root;
}

void AVLTree::inorder(const std::shared_ptr<AVLNode>& node) const {
    if (!node) return;
    inorder(node->left);
    for (const auto& s : node->students)
        std::cout << "ID: " << s->id << " | GPA: " << s->gpa << " | Name: " << s->name << "\n";
    inorder(node->right);
}

void AVLTree::saveRec(std::ofstream& out, const std::shared_ptr<AVLNode>& node) const {
    if (!node) return;
    for (const auto& s : node->students)
        out << s->id << ' ' << s->gpa << ' ' << s->name << '\n';
    saveRec(out, node->left);
    saveRec(out, node->right);
}

void AVLTree::insert(std::shared_ptr<Student> s) {
    root = insertNode(root, s);
}

void AVLTree::deleteStudent(int id) {
    root = deleteStudentRec(root, id);
}

void AVLTree::display() const {
    if (!root)
        std::cout << "No students.\n";
    else
        inorder(root);
}

void AVLTree::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Error opening file for writing.\n";
        return;
    }
    saveRec(out, root);
}

void AVLTree::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "File not found, starting fresh.\n";
        return;
    }
    root.reset();
    int id; double gpa; std::string name;
    while (in >> id >> gpa >> name)
        insert(std::make_shared<Student>(id, gpa, name));
}
