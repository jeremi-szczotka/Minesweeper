// BoardGen.hpp
#pragma once

#include "Cell.h"
#include <vector>
#include <memory>
#include <random>
#include <utility>

class BoardGen {
private:
    int rows;
    int cols;
    int bombCount;

    std::vector<std::vector<std::unique_ptr<Cell>>> board;

    void placeBombs();
    void incrementNeighbor(int r, int c);

public:
    BoardGen(int rows, int cols, int bombs);

    BoardGen(const BoardGen&) = delete;            // Block copy constructor
    BoardGen& operator=(const BoardGen&) = delete;  // Block copy assignment

    Cell* getCell(int row, int col) const;
    int getRows() const;
    int getCols() const;
};
