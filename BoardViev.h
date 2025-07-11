﻿#pragma once

#include <SFML/Graphics.hpp>
#include "BoardGen.h"

class BoardViev {

public:
    BoardViev(BoardGen* board, int rows, int cols);

    void handleClick(const sf::Vector2f& mousePos);
    void handleRightClick(const sf::Vector2f& mousePos); 
    void draw(sf::RenderWindow& window);
    bool isReady() const;
    bool isGameOver() const { return gameOver; }
    bool isGameWon()  const { return gameWon; }
    void resetFirstClick() { firstClick = true; }
    bool firstClick = true;

private:
    bool gameOver = false;
    bool gameWon = false;
    int  revealedCells = 0;      
    BoardGen* board;
    int cellSize = 32;
    bool ready = false;
    sf::Font font;

    void revealRecursive(int row, int col);
};
