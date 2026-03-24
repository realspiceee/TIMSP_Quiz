#include "Tested.h"

#include <fstream>
#include <iostream>

Tested::Tested(const std::string& name) : name_(name) {}

int Tested::startCategory(Category& category) {
    const int score = category.start();
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
