#pragma once

#include "Question.h"

#include <string>
#include <vector>

class Category {
public:
    Category() = default;
    explicit Category(const std::string& name);

    const std::string& getName() const;
    void setName(const std::string& name);

    bool isFinished() const;
    bool isActive() const;

    const std::vector<Question>& getQuestions() const;
    std::vector<Question>& getQuestions();

    void showInfo() const;
    int start();

    void addQuestion();
    void removeQuestion(int index);
    void editQuestion(int index);

private:
    std::string name_;
    std::vector<Question> questions_;
    bool is_finished_ = false;
    bool is_active_ = true;
};
