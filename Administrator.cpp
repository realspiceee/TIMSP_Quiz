#include "Administrator.h"

#include <iostream>

Administrator::Administrator(const std::string& name) : name_(name) {}

void Administrator::addCategory(std::vector<Category>& categories, const std::string& categoryName) const {
    for (const auto& c : categories) {
        if (c.getName() == categoryName) {
            std::cout << "Category already exists.\n";
            return;
        }
    }

    categories.emplace_back(categoryName);
    std::cout << "Category added by administrator " << name_ << ".\n";
}

void Administrator::removeCategory(std::vector<Category>& categories, const std::string& categoryName) const {
    if (categories.empty()) {
        std::cout << "Category list is empty. Nothing to remove.\n";
        return;
    }

    for (auto it = categories.begin(); it != categories.end(); ++it) {
        if (it->getName() == categoryName) {
            categories.erase(it);
            std::cout << "Category removed.\n";
            return;
        }
    }

    std::cout << "Category not found.\n";
}

void Administrator::editCategory(std::vector<Category>& categories, const std::string& categoryName) const {
    for (auto& c : categories) {
        if (c.getName() == categoryName) {
            std::cout << "1) Rename\n2) Add question\n3) Remove question\n4) Edit question\nSelect: ";
            int choice = 0;
            std::cin >> choice;

            if (choice == 1) {
                std::string newName;
                std::cout << "New category name: ";
                std::getline(std::cin >> std::ws, newName);
                c.setName(newName);
            } else if (choice == 2) {
                c.addQuestion();
            } else if (choice == 3) {
                int index = 0;
                std::cout << "Question index (from 1): ";
                std::cin >> index;
                c.removeQuestion(index - 1);
            } else if (choice == 4) {
                int index = 0;
                std::cout << "Question index (from 1): ";
                std::cin >> index;
                c.editQuestion(index - 1);
            } else {
                std::cout << "Unknown option.\n";
            }
            return;
        }
    }

    std::cout << "Category not found.\n";
}
