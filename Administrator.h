#pragma once

#include "Category.h"

#include <string>
#include <vector>

class Administrator {
public:
    explicit Administrator(const std::string& name);

    void addCategory(std::vector<Category>& categories, const std::string& categoryName) const;
    void removeCategory(std::vector<Category>& categories, const std::string& categoryName) const;
    void editCategory(std::vector<Category>& categories, const std::string& categoryName) const;

private:
    std::string name_;
};
