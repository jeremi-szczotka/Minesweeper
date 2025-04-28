#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
#include <vector>

class Menu {
private:
    std::vector<std::unique_ptr<Button>> buttons;
    sf::Font font;

public:
    Menu(float width, float height);

    void update(const sf::Vector2f& mousePos, const sf::Event& event);
    void draw(sf::RenderWindow& window);
    int getClickedIndex(const sf::Vector2f& mousePos, const sf::Event& event) const;
};
