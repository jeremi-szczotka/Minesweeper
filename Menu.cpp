#include "Menu.h"
#include <iostream>

Menu::Menu(float width, float height) {
    if (!font.loadFromFile("Starjedi.ttf")) {
        std::cerr << "Could not load the font!\n";
    }

    // Przycisk "New Game"
    buttons.emplace_back(std::make_unique<Button>(
        width / 2 - 100, 100, 200, 50, "New Game",
        sf::Color::Blue, sf::Color::Yellow, sf::Color::Red));

    // Przycisk "Load Game" (na razie nieaktywny)
    buttons.emplace_back(std::make_unique<Button>(
        width / 2 - 100, 180, 200, 50, "Load Game",
        sf::Color(50, 50, 50), sf::Color::White, sf::Color::White)); // Wygaszony
}

void Menu::update(const sf::Vector2f& mousePos, const sf::Event& event) {
    for (auto& button : buttons) {
        button->update(mousePos, event);
    }
}

void Menu::draw(sf::RenderWindow& window) {
    for (auto& button : buttons) {
        button->draw(window);
    }
}

int Menu::getClickedIndex(const sf::Vector2f& mousePos, const sf::Event& event) const {
    for (size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i]->isClicked(mousePos, event)) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
