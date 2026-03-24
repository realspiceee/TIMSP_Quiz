#pragma once

#include <string>
#include <vector>

class Question {
public:
    Question() = default;
    Question(const std::string& text,
             const std::vector<std::string>& answers,
             int correctIndex,
             int points);

    bool isValid() const;
    void edit();

    const std::string& getText() const;
    const std::vector<std::string>& getAnswers() const;
    int getCorrectIndex() const;
    int getPoints() const;

    void setText(const std::string& text);
    void setAnswers(const std::vector<std::string>& answers);
    void setCorrectIndex(int index);
    void setPoints(int points);

private:
    std::string text_;
    std::vector<std::string> answers_;
    int correct_index_ = -1;
    int points_ = 0;
};
