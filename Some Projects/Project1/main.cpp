#include "AVLTree.h"
#include <limits>

int main() {
    AVLTree tree;
    tree.load("students.txt");

    while (true) {
        std::cout << "\n--- Student Manager ---\n";
        std::cout << "1. Add Student\n2. Delete Student\n3. Find by ID\n4. Display All\n5. Save & Exit\n> ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) {
            int id; std::string name; double gpa;
            std::cout << "Enter ID: ";
            while (!(std::cin >> id) || id < 0) {
                std::cout << "Invalid ID. Try again: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            std::cin.ignore(); // clear newline
            std::cout << "Enter Name: ";
            std::getline(std::cin, name);

            std::cout << "Enter GPA: ";
            while (!(std::cin >> gpa) || gpa < 0.0 || gpa > 4.0) {
                std::cout << "Invalid GPA. Try again (0.0–4.0): ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            tree.insert(Student(id, name, gpa));
        }

        else if (choice == 2) {
            int id;
            std::cout << "Enter ID to delete: ";
            std::cin >> id;
            tree.deleteStudentById(id);
        }

        else if (choice == 3) {
            int id;
            std::cout << "Enter ID to search: ";
            std::cin >> id;
            const Student* s = tree.findStudentById(id);
            if (s) s->display();
            else std::cout << "❌ Not found.\n";
        }

        else if (choice == 4) tree.displayAll();

        else if (choice == 5) {
            tree.save("students.txt");
            std::cout << "Goodbye!\n";
            break;
        }
    }
    return 0;
}
