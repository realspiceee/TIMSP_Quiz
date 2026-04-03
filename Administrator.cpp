#include "Administrator.h"

#include <iostream>

namespace {
    void printQuestionsShort(const Category& category) {
        const auto& questions = category.getQuestions();
        if (questions.empty()) {
            std::cout << "This category has no questions yet.\n";
            return;
        }

        std::cout << "Questions in category '" << category.getName() << "':\n";
        for (size_t i = 0; i < questions.size(); ++i) {
            std::cout << i + 1 << ") " << questions[i].getText() << "\n";
        }
    }
} // namespace

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
            std::cout << "1) Rename\n2) Add question\n3) Remove question\n4) Edit question\n0) Cancel\nSelect: ";
            int choice = 0;
            std::cin >> choice;

            if (choice == 0) {
                std::cout << "Action cancelled.\n";
                return;
            }

            if (choice == 1) {
                std::string newName;
                std::cout << "New category name (or empty to cancel): ";
                std::getline(std::cin >> std::ws, newName);
                if (newName.empty()) {
                    std::cout << "Action cancelled.\n";
                    return;
                }
                c.setName(newName);
            }
            else if (choice == 2) {
                c.addQuestion();
            }
            else if (choice == 3) {
                printQuestionsShort(c);
                if (c.getQuestions().empty()) return;

                int index = 0;
                std::cout << "Question number to remove (0 to cancel): ";
                std::cin >> index;
                if (index == 0) {
                    std::cout << "Action cancelled.\n";
                    return;
                }
                c.removeQuestion(index - 1);
            }
            else if (choice == 4) {
                printQuestionsShort(c);
                if (c.getQuestions().empty()) return;

                int index = 0;
                std::cout << "Question number to edit (0 to cancel): ";
                std::cin >> index;
                if (index == 0) {
                    std::cout << "Action cancelled.\n";
                    return;
                }
                c.editQuestion(index - 1);
            }
            else {
                std::cout << "Unknown option.\n";
            }
            return;
        }
    }

    std::cout << "Category not found.\n";
}

void Administrator::removeCategoryByIndex(std::vector<Category>& categories, int index) const {
    if (categories.empty()) {
        std::cout << "Category list is empty. Nothing to remove.\n";
        return;
    }

    if (index < 0 || index >= static_cast<int>(categories.size())) {
        std::cout << "Invalid category number.\n";
        return;
    }

    categories.erase(categories.begin() + index);
    std::cout << "Category removed.\n";
}

void Administrator::editCategoryByIndex(std::vector<Category>& categories, int index) const {
    if (categories.empty()) {
        std::cout << "Category list is empty. Nothing to edit.\n";
        return;
    }

    if (index < 0 || index >= static_cast<int>(categories.size())) {
        std::cout << "Invalid category number.\n";
        return;
    }

    editCategory(categories, categories[index].getName());
}