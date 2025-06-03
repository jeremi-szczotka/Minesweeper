#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "DifficultyMenu.h"
#include "BoardViev.h"
#include "BoardGen.h"
#include <memory>
#include <iostream>

int main() {
    const float cellSize = 32.0f;

    // === MENU WINDOW ===
    {
        sf::RenderWindow menuWindow(sf::VideoMode(800, 600), "Minesweeper - Menu");
        Menu menu(menuWindow.getSize().x, menuWindow.getSize().y);

        while (menuWindow.isOpen()) {
            sf::Event event;
            sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(menuWindow));

            while (menuWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    menuWindow.close();
                    return 0;
                }

                menu.update(mousePos, event);
                int clicked = menu.getClickedIndex(mousePos, event);
                if (clicked == 0) { // "New Game"
                    menuWindow.close();
                }
            }

            menuWindow.clear();
            menu.draw(menuWindow);
            menuWindow.display();
        }
    }

    // === DIFFICULTY WINDOW ===
    int rows = 0, cols = 0, bombs = 0;
    {
        sf::RenderWindow difficultyWindow(sf::VideoMode(400, 300), "Select Difficulty");
        DifficultyMenu difficultyMenu(difficultyWindow.getSize().x, difficultyWindow.getSize().y);

        while (difficultyWindow.isOpen()) {
            sf::Event event;
            sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(difficultyWindow));

            while (difficultyWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    difficultyWindow.close();
                    return 0;
                }

                difficultyMenu.update(mousePos, event);
                int difficulty = difficultyMenu.getClickedIndex(mousePos, event);
                if (difficulty != -1) {
                    if (difficulty == 0) { rows = cols = 8; bombs = 10; }
                    else if (difficulty == 1) { rows = cols = 16; bombs = 40; }
                    else if (difficulty == 2) { rows = cols = 32; bombs = 99; }

                    difficultyWindow.close();
                }
            }

            difficultyWindow.clear();
            difficultyMenu.draw(difficultyWindow);
            difficultyWindow.display();
        }
    }

    // === GAME WINDOW ===
    {
        unsigned int winW = static_cast<unsigned int>(cols * cellSize);
        unsigned int winH = static_cast<unsigned int>(rows * cellSize);
        sf::RenderWindow gameWindow(sf::VideoMode(winW, winH), "Minesweeper - Game");

        std::unique_ptr<BoardGen> board = std::make_unique<BoardGen>(rows, cols, bombs);
        std::unique_ptr<BoardViev> boardView = std::make_unique<BoardViev>(board.get(), rows, cols);

        while (gameWindow.isOpen()) {
            sf::Event event;
            while (gameWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    gameWindow.close();
                    return 0;
                }

                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2f mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(gameWindow));

                    if (event.mouseButton.button == sf::Mouse::Left) {
                        boardView->handleClick(mousePos);
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right) {
                        boardView->handleRightClick(mousePos);
                    }
                }
            }

            gameWindow.clear();
            boardView->draw(gameWindow);
            gameWindow.display();
        }
    }

    return 0;
}
