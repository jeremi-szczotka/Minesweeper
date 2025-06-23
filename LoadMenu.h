#pragma once
#include "Button.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ranges>

class LoadMenu {
private:
    std::vector<Button> buttons;
    sf::Font font;

public:
    LoadMenu(const std::vector<std::string>& filenames, float width);

    void update(const sf::Vector2f& mousePos, const sf::Event& event);
    void draw(sf::RenderWindow& window);
    std::string getSelectedFile(const sf::Vector2f& mousePos, const sf::Event& event) const;
};


