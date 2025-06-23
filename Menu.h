#pragma once
#include <vector>
#include "Button.h"
#include <SFML/Graphics.hpp>
#include "DifficultyMenu.h"
#include "BoardViev.h"
#include "BoardGen.h"
#include "Load.h"
#include "Button.h"
#include <memory>
#include "LoadMenu.h"
#include <iostream>
#include <time.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>


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
