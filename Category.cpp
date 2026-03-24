#include "Category.h"

#include <iostream>

Category::Category(const std::string& name) : name_(name) {}

const std::string& Category::getName() const { return name_; }
void Category::setName(const std::string& name) { name_ = name; }

bool Category::isFinished() const { return is_finished_; }
bool Category::isActive() const { return is_active_; }

const std::vector<Question>& Category::getQuestions() const { return questions_; }
std::vector<Question>& Category::getQuestions() { return questions_; }

void Category::showInfo() const {
    std::cout << "Category: " << name_ << "\n";
    std::cout << "Questions: " << questions_.size() << "\n";
    std::cout << "Status: " << (is_active_ ? "active" : "inactive") << "\n";
}

int Category::start() {
    if (questions_.empty()) {
        std::cout << "This category has no questions.\n";
        return 0;
    }

    is_finished_ = false;
    int score = 0;

    for (size_t i = 0; i < questions_.size(); ++i) {
        const auto& q = questions_[i];
        if (!q.isValid()) {
            std::cout << "Skipping invalid question #" << i + 1 << "\n";
            continue;
        }

        std::cout << "\nQuestion " << i + 1 << ": " << q.getText() << "\n";
        const auto& answers = q.getAnswers();
        for (size_t a = 0; a < answers.size(); ++a) {
            std::cout << a + 1 << ") " << answers[a] << "\n";
        }

        int choice = 0;
        std::cout << "Your answer: ";
        std::cin >> choice;

        if (choice - 1 == q.getCorrectIndex()) {
            score += q.getPoints();
            std::cout << "Correct! +" << q.getPoints() << "\n";
        } else {
            std::cout << "Wrong answer.\n";
        }
    }

    is_finished_ = true;
    return score;
}

void Category::addQuestion() {
    std::string text;
    std::cout << "Question text: ";
    std::getline(std::cin >> std::ws, text);

    int answersCount = 0;
    std::cout << "Answer count: ";
    std::cin >> answersCount;

    std::vector<std::string> answers;
    answers.reserve(answersCount);
    for (int i = 0; i < answersCount; ++i) {
        std::cout << "Answer " << i + 1 << ": ";
        std::string answer;
        std::getline(std::cin >> std::ws, answer);
        answers.push_back(answer);
    }

    int correct = 0;
    std::cout << "Correct answer number (1.." << answersCount << "): ";
    std::cin >> correct;

    int points = 0;
    std::cout << "Question points: ";
    std::cin >> points;

    Question q(text, answers, correct - 1, points);
    if (!q.isValid()) {
        std::cout << "Invalid question. Not added.\n";
        return;
    }

    questions_.push_back(q);
    std::cout << "Question added.\n";
}

void Category::removeQuestion(int index) {
    if (index < 0 || index >= static_cast<int>(questions_.size())) {
        std::cout << "Invalid question index.\n";
        return;
    }

    questions_.erase(questions_.begin() + index);
    std::cout << "Question removed.\n";
}

void Category::editQuestion(int index) {
    if (index < 0 || index >= static_cast<int>(questions_.size())) {
        std::cout << "Invalid question index.\n";
        return;
    }

    questions_[index].edit();
}
