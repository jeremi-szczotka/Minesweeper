#pragma once
#include <vector>
#include <memory>
#include "Cell.h"

class BoardGen {
public:
    BoardGen(int rows, int cols, int bombs);

    BoardGen(const BoardGen&) = delete;
    BoardGen& operator=(const BoardGen&) = delete;

    
    BoardGen(BoardGen&&) noexcept = default;
    BoardGen& operator=(BoardGen&&) noexcept = default;

    void generate(int startRow, int startCol);
    Cell* getCell(int row, int col) const;
    int getRows() const;
    void revealCell(int row, int col);  // nowa metoda, bez zmiany starej logiki
    bool isGameOver() const;
    bool isGameWon() const;
    int getCols() const;
    const std::vector<std::vector<std::unique_ptr<Cell>>>& getGrid() const;
    void setGrid(std::vector<std::vector<std::unique_ptr<Cell>>> newGrid);

private:
    int rows;
    int cols;
    int bombs;
    bool gameOver = false;
    bool gameWon = false;
    int revealedCells = 0;

    std::vector<std::vector<std::unique_ptr<Cell>>> grid;

    void placeBombs(int startRow, int startCol);
    void calculateNeighbors();
    bool isValid(int row, int col) const;
};
