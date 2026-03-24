#pragma once

#include "Category.h"

#include <string>
#include <unordered_map>
#include <vector>

class Tested {
public:
    explicit Tested(const std::string& name);

    int startCategory(Category& category);
    void getResults() const;
    void exportResultsToFile(const std::string& filename) const;

private:
    std::string name_;
    std::unordered_map<std::string, int> category_scores_;
    int total_points_ = 0;
};
