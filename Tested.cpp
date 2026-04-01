#include "Tested.h"

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
} // namespace

Tested::Tested(const std::string& name) : name_(name) {}

void Tested::setName(const std::string& name) { name_ = name; }
const std::string& Tested::getName() const { return name_; }

int Tested::startCategory(Category& category) {
    const int score = category.start();

    auto it = category_scores_.find(category.getName());
    if (it != category_scores_.end()) {
        total_points_ -= it->second;
    }

    category_scores_[category.getName()] = score;
    total_points_ += score;
    return score;
}

void Tested::getResults() const {
    if (category_scores_.empty()) {
        std::cout << "You have not completed any category yet.\n";
        return;
    }

    std::cout << "\nTested user: " << name_ << "\n";
    for (const auto& [category, score] : category_scores_) {
        std::cout << "- " << category << ": " << score << " points\n";
    }
    std::cout << "Total: " << total_points_ << " points\n";
}

void Tested::exportResultsToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) {
        std::cout << "Cannot open file for writing: " << filename << "\n";
        return;
    }

    out << "{\n";
    out << "  \"tested\": \"" << name_ << "\",\n";
    out << "  \"total_points\": " << total_points_ << ",\n";
    out << "  \"results\": [\n";

    bool first = true;
    for (const auto& [category, score] : category_scores_) {
        if (!first) out << ",\n";
        out << "    { \"category\": \"" << category << "\", \"score\": " << score << " }";
        first = false;
    }

    out << "\n  ]\n";
    out << "}\n";

    std::cout << "Results saved to " << filename << "\n";
}

bool Tested::importResultsFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cout << "Cannot open results file: " << filename << "\n";
        return false;
    }

    std::unordered_map<std::string, int> importedScores;
    int importedTotal = 0;
    std::string importedName = name_;

    std::string line;
    while (std::getline(in, line)) {
        if (line.find("\"tested\"") != std::string::npos) {
            const std::string n = getStringValue(line, "tested");
            if (!n.empty()) importedName = n;
            continue;
        }

        if (line.find("\"total_points\"") != std::string::npos) {
            importedTotal = getIntValue(line, "total_points");
            continue;
        }

        if (line.find("\"category\"") != std::string::npos && line.find("\"score\"") != std::string::npos) {
            const std::string category = getStringValue(line, "category");
            const int score = getIntValue(line, "score");
            if (!category.empty()) {
                importedScores[category] = score;
            }
        }
    }

    category_scores_ = importedScores;
    total_points_ = importedTotal;
    name_ = importedName;

    std::cout << "Results imported from " << filename << " for user: " << name_ << "\n";
    return true;
}