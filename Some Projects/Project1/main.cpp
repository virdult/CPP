#include "AVLTree.h"
#include "utils.h"
#include <memory>

int main() {
    AVLTree tree;
    tree.loadFromFile("students.txt");

    while (true) {
        std::cout << "\n--- Student Manager ---\n"
                  << "1. Add Student\n"
                  << "2. Delete Student\n"
                  << "3. Display Students\n"
                  << "4. Save & Exit\n"
                  << "Enter choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) {
            int id; double gpa; std::string name;
            getValidatedInt("Enter ID: ", id);
            getValidatedDouble("Enter GPA: ", gpa);
            std::cout << "Enter Name: ";
            std::cin >> name;
            tree.insert(std::make_shared<Student>(id, gpa, name));
        } else if (choice == 2) {
            int id;
            getValidatedInt("Enter ID to delete: ", id);
            tree.deleteStudent(id);
        } else if (choice == 3) {
            tree.display();
        } else if (choice == 4) {
            tree.saveToFile("students.txt");
            std::cout << "Data saved. Goodbye!\n";
            break;
        } else {
            std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}
