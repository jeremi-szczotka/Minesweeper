#pragma once
#include <vector>
#include <memory>
#include "Cell.h"

class BoardGen {
public:
    BoardGen(int rows, int cols, int bombs);

    // ZABLOKOWANE KOPIOWANIE – usuwa C2280
    BoardGen(const BoardGen&) = delete;
    BoardGen& operator=(const BoardGen&) = delete;

    // OPCJONALNE PRZENOSZENIE (jeœli chcesz kiedyœ std::move)
    BoardGen(BoardGen&&) noexcept = default;
    BoardGen& operator=(BoardGen&&) noexcept = default;

    void generate(int startRow, int startCol);
    Cell* getCell(int row, int col) const;
    int getRows() const;
    int getCols() const;

private:
    int rows;
    int cols;
    int bombs;
    std::vector<std::vector<std::unique_ptr<Cell>>> grid;

    void placeBombs(int startRow, int startCol);
    void calculateNeighbors();
    bool isValid(int row, int col) const;
};
