#include "LoadMenu.h"



LoadMenu::LoadMenu(const std::vector<std::string>& filenames, float width) {
    if (!font.loadFromFile("Starjedi.ttf")) {
        std::cerr << "Could not load font!\n";
    }

    float y = 50.f;
    for (const auto& name : filenames) {
        buttons.emplace_back(
            width / 2 - 100, y, 200, 40, name,
            sf::Color::Blue, sf::Color::Yellow, sf::Color::Red
        );
        buttons.back().setFont(font);
        y += 50;
    }
}

void LoadMenu::update(const sf::Vector2f& mousePos, const sf::Event& event) {
    for (auto& btn : buttons) {
        btn.update(mousePos, event);
    }
}

void LoadMenu::draw(sf::RenderWindow& window) {
    for (auto& btn : buttons) {
        btn.draw(window);
    }
}

std::string LoadMenu::getSelectedFile(const sf::Vector2f& mousePos, const sf::Event& event) const {
    for (const auto& btn : buttons) {
        if (btn.isClicked(mousePos, event)) {
            return btn.getText();
        }
    }
    return "";
}

