#include "MainMenu.h"

#include <cstdlib>
#include <iostream>

int main() {
    system("chcp 1251 > nul");

    std::cout << "Testing System (C++17)\n";

    MainMenu menu;
    menu.loadCategories("categories.json");
    menu.run();
    menu.saveCategories("categories.json");

    return 0;
}
