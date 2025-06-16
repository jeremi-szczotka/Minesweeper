#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font font;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;

    enum ButtonState { IDLE = 0, HOVER, ACTIVE };
    ButtonState buttonState;
public:

    Button(float x, float y, float width, float height, const std::string& textStr,
        const sf::Color& idle, const sf::Color& hover, const sf::Color& active,
        unsigned int charSize = 20);

    void update(const sf::Vector2f& mousePos, const sf::Event& event);
    void draw(sf::RenderWindow& window);
    bool isClicked(const sf::Vector2f& mousePos, const sf::Event& event) const;
    std::string getText() const;
    void setFont(const sf::Font& font);
};
