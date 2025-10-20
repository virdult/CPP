#include "AVLTree.h"
#include <sstream>
#include <algorithm>

int AVLTree::getHeight(const std::unique_ptr<Node>& n) const {
    return n ? n->height : 0;
}

int AVLTree::getBalance(const std::unique_ptr<Node>& n) const {
    return n ? getHeight(n->left) - getHeight(n->right) : 0;
}

void AVLTree::updateHeight(Node* n) {
    n->height = 1 + std::max(getHeight(n->left), getHeight(n->right));
}

std::unique_ptr<AVLTree::Node> AVLTree::rotateRight(std::unique_ptr<Node> y) {
    std::unique_ptr<Node> x = std::move(y->left);
    std::unique_ptr<Node> T2 = std::move(x->right);

    x->right = std::move(y);
    x->right->left = std::move(T2);

    updateHeight(x->right.get());
    updateHeight(x.get());

    return x;
}

std::unique_ptr<AVLTree::Node> AVLTree::rotateLeft(std::unique_ptr<Node> x) {
    std::unique_ptr<Node> y = std::move(x->right);
    std::unique_ptr<Node> T2 = std::move(y->left);

    y->left = std::move(x);
    y->left->right = std::move(T2);

    updateHeight(y->left.get());
    updateHeight(y.get());

    return y;
}

bool AVLTree::idExists(const Node* node, int id) const {
    if (!node) return false;
    for (auto& s : node->students)
        if (s.id == id) return true;
    return idExists(node->left.get(), id) || idExists(node->right.get(), id);
}

std::unique_ptr<AVLTree::Node> AVLTree::insertNode(std::unique_ptr<Node> node, const Student& s) {
    if (!node) return std::make_unique<Node>(s.gpa, s);

    if (s.gpa < node->gpa)
        node->left = insertNode(std::move(node->left), s);
    else if (s.gpa > node->gpa)
        node->right = insertNode(std::move(node->right), s);
    else {
        // same GPA — sort by ID
        auto it = std::lower_bound(node->students.begin(), node->students.end(), s.id,
            [](const Student& st, int id) { return st.id < id; });
        node->students.insert(it, s);
        return node;
    }

    updateHeight(node.get());
    int balance = getBalance(node);

    // Rotations
    if (balance > 1 && s.gpa < node->left->gpa) return rotateRight(std::move(node));
    if (balance < -1 && s.gpa > node->right->gpa) return rotateLeft(std::move(node));
    if (balance > 1 && s.gpa > node->left->gpa) {
        node->left = rotateLeft(std::move(node->left));
        return rotateRight(std::move(node));
    }
    if (balance < -1 && s.gpa < node->right->gpa) {
        node->right = rotateRight(std::move(node->right));
        return rotateLeft(std::move(node));
    }

    return node;
}

void AVLTree::insert(const Student& s) {
    if (idExists(root.get(), s.id)) {
        std::cout << "❌ ID already exists!\n";
        return;
    }
    root = insertNode(std::move(root), s);
    std::cout << "✅ Student inserted successfully.\n";
}

void AVLTree::inorder(const Node* node) const {
    if (!node) return;
    inorder(node->left.get());
    for (const auto& s : node->students) s.display();
    inorder(node->right.get());
}

void AVLTree::displayAll() const {
    if (!root) std::cout << "(empty)\n";
    else inorder(root.get());
}

AVLTree::Node* AVLTree::minValueNode(Node* node) {
    while (node->left) node = node->left.get();
    return node;
}

std::unique_ptr<AVLTree::Node> AVLTree::deleteById(std::unique_ptr<Node> node, int id) {
    if (!node) return nullptr;

    bool found = false;
    for (auto it = node->students.begin(); it != node->students.end(); ++it) {
        if (it->id == id) {
            node->students.erase(it);
            found = true;
            break;
        }
    }

    if (found) {
        if (node->students.empty()) {
            if (!node->left) return std::move(node->right);
            else if (!node->right) return std::move(node->left);
            Node* minNode = minValueNode(node->right.get());
            node->gpa = minNode->gpa;
            node->students = minNode->students;
            node->right = deleteById(std::move(node->right), minNode->students.front().id);
        }
    } else if (id < node->gpa * 1000)
        node->left = deleteById(std::move(node->left), id);
    else
        node->right = deleteById(std::move(node->right), id);

    if (!node) return nullptr;

    updateHeight(node.get());
    int balance = getBalance(node);
    if (balance > 1 && getBalance(node->left) >= 0) return rotateRight(std::move(node));
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(std::move(node->left));
        return rotateRight(std::move(node));
    }
    if (balance < -1 && getBalance(node->right) <= 0) return rotateLeft(std::move(node));
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(std::move(node->right));
        return rotateLeft(std::move(node));
    }

    return node;
}

void AVLTree::deleteStudentById(int id) {
    root = deleteById(std::move(root), id);
    std::cout << "🗑️  Deleted student (if existed)\n";
}

const Student* AVLTree::findById(const Node* node, int id) const {
    if (!node) return nullptr;
    for (const auto& s : node->students)
        if (s.id == id) return &s;
    const Student* left = findById(node->left.get(), id);
    if (left) return left;
    return findById(node->right.get(), id);
}

const Student* AVLTree::findStudentById(int id) const {
    return findById(root.get(), id);
}

void AVLTree::saveToFile(const Node* node, std::ofstream& file) const {
    if (!node) return;
    for (const auto& s : node->students)
        file << s.id << ',' << s.name << ',' << s.gpa << '\n';
    saveToFile(node->left.get(), file);
    saveToFile(node->right.get(), file);
}

void AVLTree::save(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "❌ Failed to open file for saving.\n";
        return;
    }
    saveToFile(root.get(), file);
    std::cout << "💾 Data saved to file.\n";
}

void AVLTree::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return;

    root.reset(); // clear tree
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, name, gpaStr;
        if (!std::getline(ss, idStr, ',')) continue;
        if (!std::getline(ss, name, ',')) continue;
        if (!std::getline(ss, gpaStr, ',')) continue;
        Student s(std::stoi(idStr), name, std::stod(gpaStr));
        insert(s);
    }
    std::cout << "📂 Data loaded.\n";
}
