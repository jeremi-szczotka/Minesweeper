#include "Button.h"
#include <iostream>

Button::Button(float x, float y, float width, float height, const std::string& textStr,
    const sf::Color& idle, const sf::Color& hover, const sf::Color& active,
    unsigned int charSize)
{
    shape.setPosition(x, y);
    shape.setSize({ width, height });
    idleColor = idle;
    hoverColor = hover;
    activeColor = active;
    buttonState = IDLE;
    shape.setFillColor(idleColor);

    if (!font.loadFromFile("Starjedi.ttf")) {
        std::cerr << "Error\n";
    }

    text.setFont(font);
    text.setString(textStr);
    text.setCharacterSize(charSize);
    text.setFillColor(sf::Color::White);
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.width / 2, textBounds.height / 2);
    text.setPosition(x + width / 2, y + height / 2 - 5);
}

void Button::update(const sf::Vector2f& mousePos, const sf::Event& event) {
    buttonState = IDLE;

    if (shape.getGlobalBounds().contains(mousePos)) {
        buttonState = HOVER;

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left)
        {
            buttonState = ACTIVE;
        }
    }

    switch (buttonState)
    {
    case IDLE:
        shape.setFillColor(idleColor);
        break;
    case HOVER:
        shape.setFillColor(hoverColor);
        break;
    case ACTIVE:
        shape.setFillColor(activeColor);
        break;
    }
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

bool Button::isClicked(const sf::Vector2f& mousePos, const sf::Event& event) const {
    return (shape.getGlobalBounds().contains(mousePos) &&
        event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left);
}
