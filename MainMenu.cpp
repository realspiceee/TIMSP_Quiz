#include "MainMenu.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace {
std::string getStringValue(const std::string& line, const std::string& key) {
    const std::string token = "\"" + key + "\"";
    const size_t keyPos = line.find(token);
    if (keyPos == std::string::npos) return {};

    const size_t first = line.find('"', keyPos + token.size());
    const size_t second = (first == std::string::npos) ? std::string::npos : line.find('"', first + 1);
    if (first == std::string::npos || second == std::string::npos) return {};

    return line.substr(first + 1, second - first - 1);
}

int getIntValue(const std::string& line, const std::string& key) {
    const std::string token = "\"" + key + "\"";
    const size_t keyPos = line.find(token);
    if (keyPos == std::string::npos) return 0;

    const size_t colon = line.find(':', keyPos + token.size());
    if (colon == std::string::npos) return 0;

    std::stringstream ss(line.substr(colon + 1));
    int value = 0;
    ss >> value;
    return value;
}

std::vector<std::string> parseAnswers(const std::string& line) {
    std::vector<std::string> answers;

    const size_t left = line.find('[');
    const size_t right = line.find(']');
    if (left == std::string::npos || right == std::string::npos || right <= left) {
        return answers;
    }

    const std::string data = line.substr(left + 1, right - left - 1);
    size_t pos = 0;
    while (true) {
        const size_t q1 = data.find('"', pos);
        if (q1 == std::string::npos) break;
        const size_t q2 = data.find('"', q1 + 1);
        if (q2 == std::string::npos) break;

        answers.push_back(data.substr(q1 + 1, q2 - q1 - 1));
        pos = q2 + 1;
    }

    return answers;
}
} // namespace

MainMenu::MainMenu() : admin_("Admin"), tested_("User") {}

void MainMenu::run() {
    while (true) {
        std::cout << "\n===== MAIN MENU =====\n";
        std::cout << "1) Login\n";
        std::cout << "2) Show categories\n";
        std::cout << "3) Run module tests\n";
        std::cout << "0) Exit\n";
        std::cout << "Select: ";

        int choice = 0;
        std::cin >> choice;

        if (choice == 1) {
            login();
        } else if (choice == 2) {
            printCategories();
        } else if (choice == 3) {
            runModuleTests();
        } else if (choice == 0) {
            return;
        } else {
            std::cout << "Unknown menu item.\n";
        }
    }
}

void MainMenu::loadCategories(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cout << "Categories file not found: " << filename << ". It will be created later.\n";
        return;
    }

    categories_.clear();
    Category* current = nullptr;
    std::string line;

    while (std::getline(in, line)) {
        if (line.find("\"name\"") != std::string::npos && line.find("\"questions\"") == std::string::npos) {
            const std::string name = getStringValue(line, "name");
            if (!name.empty()) {
                categories_.emplace_back(name);
                current = &categories_.back();
            }
            continue;
        }

        if (line.find("\"text\"") != std::string::npos && current != nullptr) {
            const std::string text = getStringValue(line, "text");
            const std::vector<std::string> answers = parseAnswers(line);
            const int correct = getIntValue(line, "correct");
            const int points = getIntValue(line, "points");

            Question q(text, answers, correct, points);
            if (q.isValid()) {
                current->getQuestions().push_back(q);
            }
        }
    }

    std::cout << "Loaded categories: " << categories_.size() << "\n";
}

void MainMenu::saveCategories(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) {
        std::cout << "Cannot save categories into file: " << filename << "\n";
        return;
    }

    out << "{\n  \"categories\": [\n";
    for (size_t i = 0; i < categories_.size(); ++i) {
        const auto& category = categories_[i];
        out << "    {\n";
        out << "      \"name\": \"" << category.getName() << "\",\n";
        out << "      \"questions\": [\n";

        const auto& questions = category.getQuestions();
        for (size_t q = 0; q < questions.size(); ++q) {
            const auto& item = questions[q];
            out << "        { \"text\": \"" << item.getText() << "\", \"answers\": [";

            const auto& answers = item.getAnswers();
            for (size_t a = 0; a < answers.size(); ++a) {
                out << "\"" << answers[a] << "\"";
                if (a + 1 < answers.size()) out << ", ";
            }

            out << "], \"correct\": " << item.getCorrectIndex() << ", \"points\": " << item.getPoints() << " }";
            if (q + 1 < questions.size()) out << ",";
            out << "\n";
        }

        out << "      ]\n";
        out << "    }";
        if (i + 1 < categories_.size()) out << ",";
        out << "\n";
    }

    out << "  ]\n}\n";
    std::cout << "Categories saved into " << filename << "\n";
}

void MainMenu::login() {
    std::cout << "Select mode: 1) Administrator 2) Tested 0) Back\n";
    int mode = 0;
    std::cin >> mode;

    if (mode == 1) {
        user_mode_ = UserMode::Admin;
        runAdminMode();
    } else if (mode == 2) {
        user_mode_ = UserMode::Tested;
        runTestedMode();
    } else {
        user_mode_ = UserMode::None;
    }
}

void MainMenu::logout() {
    user_mode_ = UserMode::None;
    std::cout << "Logged out to main menu.\n";
}

void MainMenu::runAdminMode() {
    while (user_mode_ == UserMode::Admin) {
        std::cout << "\n--- ADMIN MODE ---\n";
        std::cout << "1) Add category\n";
        std::cout << "2) Remove category\n";
        std::cout << "3) Edit category\n";
        std::cout << "4) Save categories\n";
        std::cout << "5) Show categories\n";
        std::cout << "0) Exit mode\n";
        std::cout << "Select: ";

        int choice = 0;
        std::cin >> choice;

        std::string name;
        if (choice == 1) {
            std::cout << "Category name: ";
            std::getline(std::cin >> std::ws, name);
            admin_.addCategory(categories_, name);
        } else if (choice == 2) {
            std::cout << "Category name to remove: ";
            std::getline(std::cin >> std::ws, name);
            admin_.removeCategory(categories_, name);
        } else if (choice == 3) {
            std::cout << "Category name to edit: ";
            std::getline(std::cin >> std::ws, name);
            admin_.editCategory(categories_, name);
        } else if (choice == 4) {
            saveCategories("categories.json");
        } else if (choice == 5) {
            printCategories();
        } else if (choice == 0) {
            logout();
        } else {
            std::cout << "Unknown menu item.\n";
        }
    }
}

void MainMenu::runTestedMode() {
    while (user_mode_ == UserMode::Tested) {
        std::cout << "\n--- TESTED MODE ---\n";
        std::cout << "1) Start category\n";
        std::cout << "2) Show results\n";
        std::cout << "3) Export results\n";
        std::cout << "4) Show categories\n";
        std::cout << "0) Exit mode\n";
        std::cout << "Select: ";

        int choice = 0;
        std::cin >> choice;

        if (choice == 1) {
            if (categories_.empty()) {
                std::cout << "No categories available.\n";
                continue;
            }

            printCategories();
            std::cout << "Category number: ";
            int index = 0;
            std::cin >> index;

            if (index <= 0 || index > static_cast<int>(categories_.size())) {
                std::cout << "Invalid category number.\n";
                continue;
            }

            const int score = tested_.startCategory(categories_[index - 1]);
            std::cout << "Category done. Score: " << score << "\n";
        } else if (choice == 2) {
            tested_.getResults();
        } else if (choice == 3) {
            tested_.exportResultsToFile("results.json");
        } else if (choice == 4) {
            printCategories();
        } else if (choice == 0) {
            logout();
        } else {
            std::cout << "Unknown menu item.\n";
        }
    }
}

void MainMenu::printCategories() const {
    if (categories_.empty()) {
        std::cout << "No categories.\n";
        return;
    }

    std::cout << "\nCategories:\n";
    for (size_t i = 0; i < categories_.size(); ++i) {
        std::cout << i + 1 << ") " << categories_[i].getName() << " (questions: "
                  << categories_[i].getQuestions().size() << ")\n";
    }
}

void MainMenu::runModuleTests() const {
    std::cout << "\n[Test 1] Remove from empty category list: ";
    {
        std::vector<Category> empty;
        Administrator admin("TestAdmin");
        admin.removeCategory(empty, "missing");
        std::cout << "OK\n";
    }

    std::cout << "[Test 2] Start empty category: ";
    {
        Category emptyCategory("Empty");
        const int score = emptyCategory.start();
        std::cout << (score == 0 ? "OK" : "FAIL") << "\n";
    }

    std::cout << "[Test 3] Results without testing: ";
    {
        Tested tested("EdgeCase");
        tested.getResults();
        std::cout << "OK\n";
    }
}
