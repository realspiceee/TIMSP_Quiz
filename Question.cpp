#include "Question.h"

#include <iostream>
#include <limits>

Question::Question(const std::string& text,
                   const std::vector<std::string>& answers,
                   int correctIndex,
                   int points)
    : text_(text), answers_(answers), correct_index_(correctIndex), points_(points) {}

bool Question::isValid() const {
    return !text_.empty() && !answers_.empty() && correct_index_ >= 0 &&
           correct_index_ < static_cast<int>(answers_.size()) && points_ >= 0;
}

void Question::edit() {
    std::cout << "Current question: " << text_ << "\n";
    std::cout << "Enter new question text (or empty to keep): ";

    std::string newText;
    std::getline(std::cin >> std::ws, newText);
    if (!newText.empty()) {
        text_ = newText;
    }

    std::cout << "Change answers? (1-yes, 0-no): ";
    int changeAnswers = 0;
    std::cin >> changeAnswers;

    if (changeAnswers == 1) {
        int count = 0;
        std::cout << "Answer count: ";
        std::cin >> count;

        answers_.clear();
        for (int i = 0; i < count; ++i) {
            std::cout << "Answer " << i + 1 << ": ";
            std::string ans;
            std::getline(std::cin >> std::ws, ans);
            answers_.push_back(ans);
        }

        std::cout << "Correct answer index (1.." << answers_.size() << "): ";
        std::cin >> correct_index_;
        --correct_index_;
    }

    std::cout << "New points (or -1 to keep current): ";
    int newPoints = -1;
    std::cin >> newPoints;
    if (newPoints >= 0) {
        points_ = newPoints;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

const std::string& Question::getText() const { return text_; }
const std::vector<std::string>& Question::getAnswers() const { return answers_; }
int Question::getCorrectIndex() const { return correct_index_; }
int Question::getPoints() const { return points_; }

void Question::setText(const std::string& text) { text_ = text; }
void Question::setAnswers(const std::vector<std::string>& answers) { answers_ = answers; }
void Question::setCorrectIndex(int index) { correct_index_ = index; }
void Question::setPoints(int points) { points_ = points; }
