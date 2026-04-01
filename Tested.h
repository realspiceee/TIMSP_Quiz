#pragma once

#include "Category.h"

#include <string>
#include <unordered_map>
#include <vector>

class Tested {
public:
    explicit Tested(const std::string& name);

    void setName(const std::string& name);
    const std::string& getName() const;

    int startCategory(Category& category);
    void getResults() const;
    void exportResultsToFile(const std::string& filename) const;
    bool importResultsFromFile(const std::string& filename);

private:
    std::string name_;
    std::unordered_map<std::string, int> category_scores_;
    int total_points_ = 0;
};