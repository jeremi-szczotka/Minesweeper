#include <SFML/Graphics.hpp>
#include "Menu.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Menu Example");
    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            menu.update(mousePos, event);

            int clicked = menu.getClickedIndex(mousePos, event);
            if (clicked == 0) {
                std::cout << "New Game clicked!\n";
            }
            else if (clicked == 1) {
                std::cout << "Load Game clicked!\n";
            }
        }

        window.clear();
        menu.draw(window);
        window.display();
    }

    return 0;
}
