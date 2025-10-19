#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <limits>
#include <string>

inline bool getValidatedInt(const std::string& prompt, int& out) {
    std::cout << prompt;
    while (!(std::cin >> out)) {
        std::cout << "Invalid input. Enter an integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return true;
}

inline bool getValidatedDouble(const std::string& prompt, double& out) {
    std::cout << prompt;
    while (!(std::cin >> out)) {
        std::cout << "Invalid input. Enter a number: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return true;
}

#endif
