#pragma once

#include "Administrator.h"
#include "Category.h"
#include "Tested.h"

#include <string>
#include <vector>

class MainMenu {
public:
    MainMenu();

    void run();
    void loadCategories(const std::string& filename);
    void saveCategories(const std::string& filename) const;

private:
    enum class UserMode { None, Admin, Tested };

    void login();
    void logout();

    void runAdminMode();
    void runTestedMode();

    void printCategories() const;
    void runModuleTests() const;

    std::vector<Category> categories_;
    UserMode user_mode_ = UserMode::None;

    Administrator admin_;
    Tested tested_;
};
