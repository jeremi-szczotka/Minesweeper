#include "DifficultyMenu.h"
#include <iostream>

DifficultyMenu::DifficultyMenu(float width, float height) {
    if (!font.loadFromFile("Starjedi.ttf")) {
        std::cerr << "Could not load font!\n";
    }

    difficultyButtons.emplace_back(std::make_unique<Button>(
        width / 2 - 100, 50, 200, 40, "Easy (8x8)", sf::Color::Green, sf::Color::Yellow, sf::Color::Red));

    difficultyButtons.emplace_back(std::make_unique<Button>(
        width / 2 - 100, 110, 200, 40, "Medium (16x16)", sf::Color::Magenta, sf::Color::Yellow, sf::Color::Red));

    difficultyButtons.emplace_back(std::make_unique<Button>(
        width / 2 - 100, 170, 200, 40, "Hard (32x32)", sf::Color::Cyan, sf::Color::Yellow, sf::Color::Red));
}

void DifficultyMenu::update(const sf::Vector2f& mousePos, const sf::Event& event) {
    for (auto& b : difficultyButtons)
        b->update(mousePos, event);
}

void DifficultyMenu::draw(sf::RenderWindow& window) {
    for (auto& b : difficultyButtons)
        b->draw(window);
}

int DifficultyMenu::getClickedIndex(const sf::Vector2f& mousePos, const sf::Event& event) const {
    for (size_t i = 0; i < difficultyButtons.size(); ++i) {
        if (difficultyButtons[i]->isClicked(mousePos, event)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

