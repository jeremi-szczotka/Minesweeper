#include "BoardViev.h"
#include <iostream>
#include <thread>

BoardViev::BoardViev(BoardGen* board, int rows, int cols)
    : board(board), cellSize(32), ready(true) {
    font.loadFromFile("starjedi.ttf");
}

bool BoardViev::isReady() const {
    return ready;
}

void BoardViev::handleClick(const sf::Vector2f& mousePos)
{
    if (!isReady()) return;                         

    int col = static_cast<int>(mousePos.x / cellSize);
    int row = static_cast<int>(mousePos.y / cellSize);

    if (firstClick) {                              
        board->generate(row, col);
        firstClick = false;
    }

    Cell* cell = board->getCell(row, col);
    if (!cell || cell->isRevealed() || cell->isFlagged()) return;

   
    if (cell->isBomb()) {
        cell->reveal();

       
        for (int r = 0; r < board->getRows(); ++r)
            for (int c = 0; c < board->getCols(); ++c)
                if (auto* b = board->getCell(r, c); b && b->isBomb())
                    b->reveal();

        ready = false;                             
        std::cout << "💥 BOOM! You lose!\n";
        return;
    }

   
    revealRecursive(row, col);

   
    int notRevealedSafe = 0;
    for (int r = 0; r < board->getRows(); ++r)
        for (int c = 0; c < board->getCols(); ++c) {
            Cell* tmp = board->getCell(r, c);
            if (tmp && !tmp->isBomb() && !tmp->isRevealed())
                ++notRevealedSafe;
        }

    if (notRevealedSafe == 0)
    {
        ready = false;               

        // ───────── thread ─────────
        std::thread revealThread([this]  
            {
                for (int r = 0; r < board->getRows(); ++r)
                    for (int c = 0; c < board->getCols(); ++c)
                        if (auto* b = board->getCell(r, c); b && b->isBomb())
                            b->reveal();
            });

        revealThread.detach();       

        std::cout << "🏆 You win!\n";
    }
}


void BoardViev::handleRightClick(const sf::Vector2f& mousePos) {
    if (!isReady()) return;

    int col = static_cast<int>(mousePos.x / cellSize);
    int row = static_cast<int>(mousePos.y / cellSize);

    Cell* cell = board->getCell(row, col);
    if (cell && !cell->isRevealed()) {
        cell->toggleFlag();
    }
}

void BoardViev::revealRecursive(int row, int col) {
    Cell* cell = board->getCell(row, col);
    if (!cell || cell->isRevealed() || cell->isFlagged()) return;

    cell->reveal();

    if (cell->getNeighborCount() == 0) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr != 0 || dc != 0)
                    revealRecursive(row + dr, col + dc);
            }
        }
    }
}

void BoardViev::draw(sf::RenderWindow& window) {
    for (int r = 0; r < board->getRows(); ++r) {
        for (int c = 0; c < board->getCols(); ++c) {
            Cell* cell = board->getCell(r, c);
            sf::RectangleShape rect(sf::Vector2f(cellSize - 1, cellSize - 1));
            rect.setPosition(c * cellSize, r * cellSize);

            if (cell->isRevealed()) {
                rect.setFillColor(sf::Color::White);
                window.draw(rect);

                if (cell->isBomb()) {
                    sf::CircleShape bomb(cellSize / 4);
                    bomb.setFillColor(sf::Color::Black);
                    bomb.setPosition(c * cellSize + 8, r * cellSize + 8);
                    window.draw(bomb);
                } else if (cell->getNeighborCount() > 0) {
                    sf::Text text;
                    text.setFont(font);
                    text.setString(std::to_string(cell->getNeighborCount()));
                    text.setCharacterSize(18);
                    text.setFillColor(sf::Color::Blue);
                    text.setPosition(c * cellSize + 6, r * cellSize + 2);
                    window.draw(text);
                }
            } else {
                rect.setFillColor(sf::Color(100, 100, 100));
                window.draw(rect);

                if (cell->isFlagged()) {
                    sf::Text flag;
                    flag.setFont(font);
                    flag.setString("F"); 
                    flag.setCharacterSize(20);
                    flag.setFillColor(sf::Color::Red);
                    flag.setPosition(c * cellSize + 6, r * cellSize + 2);
                    window.draw(flag);
                }
            }
        }
    }
}

