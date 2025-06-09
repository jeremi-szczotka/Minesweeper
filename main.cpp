#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "DifficultyMenu.h"
#include "BoardViev.h"
#include "BoardGen.h"
#include "Load.h"
#include "Button.h"
#include <memory>
#include <iostream>




int main()
{
    /* ─────────────────── CONFIG ─────────────────── */
    const float cellSize = 32.f;      
    const float sidebarW = 160.f;     

    int  rows = 0, cols = 0, bombs = 0;
    bool startNew = false;
    bool loadSaved = false;

    /* ─────────────────── MENU ─────────────────── */
    {
        sf::RenderWindow win({ 600, 400 }, "Minesweeper - Menu");
        Menu menu(win.getSize().x, win.getSize().y);

        while (win.isOpen()) {
            sf::Event ev;
            sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(win);

            while (win.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) { win.close(); return 0; }

                menu.update(mp, ev);
                int idx = menu.getClickedIndex(mp, ev);
                if (idx == 0) { startNew = true;  win.close(); }
                if (idx == 1) { loadSaved = true; win.close(); }
            }
            win.clear();
            menu.draw(win);
            win.display();
        }
    }

    /* ─────────────────── LOAD GAME PATH ─────────────────── */
    if (loadSaved)
    {
        try {
            auto grid = Load::load("save.json");
            rows = (int)grid.size();
            cols = (int)grid[0].size();
            bombs = 0;

            auto board = std::make_unique<BoardGen>(rows, cols, bombs);
            board->setGrid(std::move(grid));
            auto boardView = std::make_unique<BoardViev>(board.get(), rows, cols);

            unsigned winW = (unsigned)(cols * cellSize + sidebarW);
            unsigned winH = (unsigned)(rows * cellSize);
            sf::RenderWindow game({ winW, winH }, "Minesweeper - Game");

            Button saveBtn(cols * cellSize + 15.f, 15.f,      
                130, 40, "Save",
                sf::Color::Blue, sf::Color::Yellow, sf::Color::Red);

            sf::RectangleShape divider({ 2.f, (float)winH });   
            divider.setFillColor(sf::Color::Black);
            divider.setPosition(cols * cellSize, 0);

            while (game.isOpen()) {
                sf::Event ev;
                while (game.pollEvent(ev)) {
                    if (ev.type == sf::Event::Closed) { game.close(); return 0; }

                    sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(game);
                    saveBtn.update(mp, ev);

                    if (ev.type == sf::Event::MouseButtonPressed) {
                        if (ev.mouseButton.button == sf::Mouse::Left) {
                            if (saveBtn.isClicked(mp, ev)) {
                                Load::save(board->getGrid(), "save.json");
                                std::cout << "[SAVE] Game saved.\n";
                            }
                            else {
                                boardView->handleClick(mp);
                            }
                        }
                        else if (ev.mouseButton.button == sf::Mouse::Right) {
                            boardView->handleRightClick(mp);
                        }
                    }
                }
                game.clear();
                boardView->draw(game);
                game.draw(divider);
                saveBtn.draw(game);
                game.display();
            }
            return 0;
        }
        catch (...) {
            std::cerr << "[LOAD] save.json missing/corrupt – starting new game\n";
            startNew = true;
        }
    }

    /* ─────────────────── NEW GAME PATH ─────────────────── */
    if (startNew)
    {
        /* ---- DIFFICULTY ---- */
        {
            sf::RenderWindow diffWin({ 400, 300 }, "Select Difficulty");
            DifficultyMenu diffMenu(diffWin.getSize().x, diffWin.getSize().y);

            while (diffWin.isOpen()) {
                sf::Event ev;
                sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(diffWin);

                while (diffWin.pollEvent(ev)) {
                    if (ev.type == sf::Event::Closed) { diffWin.close(); return 0; }

                    diffMenu.update(mp, ev);
                    int d = diffMenu.getClickedIndex(mp, ev);
                    if (d != -1) {
                        if (d == 0) { rows = cols = 8;  bombs = 10; }
                        if (d == 1) { rows = cols = 16; bombs = 40; }
                        if (d == 2) { rows = cols = 32; bombs = 99; }
                        diffWin.close();
                    }
                }
                diffWin.clear();
                diffMenu.draw(diffWin);
                diffWin.display();
            }
        }

        /* ---- GAME ---- */
        unsigned winW = (unsigned)(cols * cellSize + sidebarW);
        unsigned winH = (unsigned)(rows * cellSize);
        sf::RenderWindow game({ winW, winH }, "Minesweeper - Game");

        auto board = std::make_unique<BoardGen>(rows, cols, bombs);
        auto boardView = std::make_unique<BoardViev>(board.get(), rows, cols);

        Button saveBtn(cols * cellSize + 15.f, 15.f, 130, 40,
            "Save game",
            sf::Color::Blue, sf::Color::Yellow, sf::Color::Red);

        sf::RectangleShape divider({ 2.f, (float)winH });
        divider.setFillColor(sf::Color::Black);
        divider.setPosition(cols * cellSize, 0);

        while (game.isOpen()) {
            sf::Event ev;
            while (game.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) { game.close(); return 0; }

                sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(game);
                saveBtn.update(mp, ev);

                if (ev.type == sf::Event::MouseButtonPressed) {
                    if (ev.mouseButton.button == sf::Mouse::Left) {
                        if (saveBtn.isClicked(mp, ev)) {
                            Load::save(board->getGrid(), "save.json");
                            std::cout << "[SAVE] Game saved.\n";
                            game.close();
                        }
                        else {
                            boardView->handleClick(mp);
                        }
                    }
                    else if (ev.mouseButton.button == sf::Mouse::Right) {
                        boardView->handleRightClick(mp);
                    }
                }
            }
            game.clear();
            boardView->draw(game);
            game.draw(divider);
            saveBtn.draw(game);
            game.display();
        }
    }
    return 0;
}
