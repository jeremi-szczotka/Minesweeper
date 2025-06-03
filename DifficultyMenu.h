#pragma once
#include "Button.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

class DifficultyMenu {
private:
    std::vector<std::unique_ptr<Button>> difficultyButtons;
    sf::Font font;

public:
    DifficultyMenu(float width, float height);

    void update(const sf::Vector2f& mousePos, const sf::Event& event);
    void draw(sf::RenderWindow& window);
    int getClickedIndex(const sf::Vector2f& mousePos, const sf::Event& event) const;
};


