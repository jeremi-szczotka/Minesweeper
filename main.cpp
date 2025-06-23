#include "Menu.h"


std::atomic<bool> loadRequested{ false };

std::vector<std::string> asyncSaves;
std::mutex asyncMtx;

std::vector<std::string> loadGames() {
    namespace fs = std::filesystem;

    auto filtered = fs::directory_iterator(Load::kSaveDir)
        | std::views::filter([](const fs::directory_entry& entry) {
        return entry.is_regular_file() &&
            entry.path().extension() == ".json";
            })
        | std::views::transform([](const fs::directory_entry& entry) {
                return entry.path().filename().string();
            });

            std::vector<std::string> saves;
            for (const auto& name : filtered) {
                saves.push_back(name);
            }
            return saves;
}

int main()
{
   
    const float cellSize = 32.f;
    const float sidebarW = 160.f;

    int  rows = 0, cols = 0, bombs = 0;
    bool startNew = false;
    bool loadSaved = false;
    
    //Thread
    std::thread loaderThread([&]() {
        while (true) {
            if (loadRequested.load()) {
          
                auto tmp = loadGames();

              
                {
                    std::lock_guard<std::mutex> lk(asyncMtx);
                    asyncSaves = std::move(tmp);
                }

                loadRequested = false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        });
    loaderThread.detach();

    
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
                if (idx == 1) {
                    
                    loadRequested = true;

                    
                    while (loadRequested) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    }

                    
                    std::vector<std::string> saves;
                    {
                        std::lock_guard<std::mutex> lk(asyncMtx);
                        saves = asyncSaves;
                    }

                    if (saves.empty()) {
                        std::cerr << "[LOAD] No saved games found!\n";
                        break;
                    }

                    sf::RenderWindow saveWin({ 600, 400 }, "Choose Save File");
                    LoadMenu loadMenu(saves, saveWin.getSize().x);
                    std::string selectedFile;

                    while (saveWin.isOpen()) {
                        sf::Event ev;
                        sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(saveWin);
                        while (saveWin.pollEvent(ev)) {
                            if (ev.type == sf::Event::Closed) return 0;

                            loadMenu.update(mp, ev);
                            selectedFile = loadMenu.getSelectedFile(mp, ev);
                            if (!selectedFile.empty()) {
                                loadSaved = true;
                                saveWin.close();
                                break;
                            }
                        }
                        saveWin.clear();
                        loadMenu.draw(saveWin);
                        saveWin.display();
                    }

                    if (selectedFile.empty()) {
                        win.close();
                    }

                    Load::selectedFile = selectedFile;
                    win.close();
                }
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
            auto grid = Load::load(Load::selectedFile);
            rows = (int)grid.size();
            cols = (int)grid[0].size();
         

            auto board = std::make_unique<BoardGen>(rows, cols, bombs);
            board->setGrid(std::move(grid));
            auto boardView = std::make_unique<BoardViev>(board.get(), rows, cols);

            unsigned winW = (unsigned)(cols * cellSize + sidebarW);
            unsigned winH = (unsigned)(rows * cellSize);
            sf::RenderWindow game({ winW, winH }, "Minesweeper - Game");

            Button saveBtn(cols * cellSize + 15.f, 15.f, 130, 40, "Save",
                sf::Color::Blue, sf::Color::Yellow, sf::Color::Red);
            Button exitBtn(cols * cellSize + 15.f, 60.f, 130, 40,"Exit",
                sf::Color::Blue, sf::Color::Yellow, sf::Color::Red);
            Button retryBtn(cols * cellSize + 15.f, 105.f, 130, 40,"Retry",
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
                    exitBtn.update(mp, ev);   
                    retryBtn.update(mp, ev);

                    if (ev.type == sf::Event::MouseButtonPressed) {
                        if (ev.mouseButton.button == sf::Mouse::Left) {
                            if (saveBtn.isClicked(mp, ev)) {
                                std::time_t now = std::time(nullptr);
                                std::tm localTime;
                                localtime_s(&localTime, &now);
                                
                                std::ostringstream ss;
                                ss << std::put_time(&localTime, "%Y-%m-%d_%H-%M-%S");
                                std::string timestamp = ss.str();
                                std::string filename = "save_" + timestamp + ".json";
                                Load::save(board->getGrid(), filename);
                                std::cout << "[SAVE] Game saved.\n";
                            }
                            if (exitBtn.isClicked(mp, ev)) {
                                game.close();
                            }
                            if (retryBtn.isClicked(mp, ev)) {
                                if (cols == 8) {
                                    bombs = 10;
                                }
                                if(cols == 16) { 
                                    bombs = 40; 
                                }
                                else {
                                    bombs = 99;
                                }
                                board = std::make_unique<BoardGen>(rows, cols, bombs);
                                boardView = std::make_unique<BoardViev>(board.get(), rows, cols);
                                

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
                exitBtn.draw(game);
                retryBtn.draw(game);
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
            "Save",
            sf::Color::Blue, sf::Color::Yellow, sf::Color::Red);

        Button exitBtn(cols * cellSize + 15.f, 60.f, 130, 40,
            "Exit",
            sf::Color::Blue, sf::Color::Yellow, sf::Color::Red);
        Button retryBtn(cols * cellSize + 15.f, 105.f, 130, 40,
            "Retry",
            sf::Color::Blue, sf::Color::Yellow, sf::Color::Red);

        sf::RectangleShape divider({ 2.f, (float)winH });
        divider.setFillColor(sf::Color::White);
        divider.setPosition(cols * cellSize, 0);

        while (game.isOpen()) {
            sf::Event ev;
            while (game.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) { game.close(); return 0; }

                sf::Vector2f mp = (sf::Vector2f)sf::Mouse::getPosition(game);
                saveBtn.update(mp, ev);
                exitBtn.update(mp, ev);
                retryBtn.update(mp, ev);

                if (ev.type == sf::Event::MouseButtonPressed) {
                    if (ev.mouseButton.button == sf::Mouse::Left) {
                        if (saveBtn.isClicked(mp, ev)) {
                            std::time_t now = std::time(nullptr);
                            std::tm localTime;
                            localtime_s(&localTime, &now);

                            std::ostringstream ss;
                            ss << std::put_time(&localTime, "%Y-%m-%d_%H-%M-%S");
                            std::string timestamp = ss.str();
                            std::string filename = "save_" + timestamp + ".json";
                            Load::save(board->getGrid(), filename);
                            std::cout << "[SAVE] Game saved.\n";
                            game.close();
                        }
                        if (exitBtn.isClicked(mp, ev)) {
                            game.close();
                        }
                        if (retryBtn.isClicked(mp, ev)) {
                            board = std::make_unique<BoardGen>(rows, cols, bombs);
                            boardView = std::make_unique<BoardViev>(board.get(), rows, cols);

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
            exitBtn.draw(game);
            retryBtn.draw(game);
            game.display();
        }
    }
    return 0;
}
