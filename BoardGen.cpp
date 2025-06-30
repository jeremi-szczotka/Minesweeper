#include "BoardGen.h"
#include "Cell.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>

BoardGen::BoardGen(int rows, int cols, int bombs)
    : rows(rows), cols(cols), bombs(bombs), grid(rows) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    for (int r = 0; r < rows; ++r) {
        grid[r].resize(cols);
        for (int c = 0; c < cols; ++c) {
            grid[r][c] = std::make_unique<NormalCell>(0);
        }
    }
}

int BoardGen::getRows() const {
    return rows;
}

int BoardGen::getCols() const {
    return cols;
}

bool BoardGen::isValid(int row, int col) const {
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

Cell* BoardGen::getCell(int row, int col) const {
    if (!isValid(row, col)) return nullptr;
    return grid[row][col].get();
}

void BoardGen::generate(int startRow, int startCol) {
    placeBombs(startRow, startCol);
    calculateNeighbors();
   
    int bombCount = 0;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            if (grid[r][c]->isBomb())
                ++bombCount;
    std::cout << "[DEBUG] Bombs placed: " << bombCount << "\n";
}

void BoardGen::placeBombs(int startRow, int startCol) {
    std::vector<std::pair<int, int>> positions;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (r == startRow && c == startCol) continue;
            positions.emplace_back(r, c);
        }
    }

    std::shuffle(positions.begin(), positions.end(), std::mt19937(static_cast<unsigned>(std::time(nullptr))));

    for (int i = 0; i < bombs && i < static_cast<int>(positions.size()); ++i) {
        int r = positions[i].first;
        int c = positions[i].second;
        grid[r][c] = std::make_unique<BombCell>();
    }
}

void BoardGen::calculateNeighbors() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c]->isBomb()) continue;

            int count = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr;
                    int nc = c + dc;
                    if (isValid(nr, nc) && grid[nr][nc]->isBomb()) {
                        ++count;
                    }
                }
            }

            if (NormalCell* normal = dynamic_cast<NormalCell*>(grid[r][c].get())) {
                normal->setNeighborCount(count);
            }
        }
    }
}
void BoardGen::revealCell(int row, int col) {
    if (gameOver || gameWon) return;

    if (!grid[row][col]->isRevealed()) {
        grid[row][col]->reveal();
        revealedCells++;

        if (grid[row][col]->isBomb()) {
            gameOver = true;
            return;
        }

        int totalCells = grid.size() * grid[0].size();
        int bombCount = 0;

        for (const auto& row : grid) {
            for (const auto& cell : row) {
                if (cell->isBomb()) bombCount++;
            }
        }

        if (revealedCells == totalCells - bombCount) {
            gameWon = true;
        }
    }
}

bool BoardGen::isGameOver() const {
    return gameOver;
}

bool BoardGen::isGameWon() const {
    return gameWon;
}

const std::vector<std::vector<std::unique_ptr<Cell>>>& BoardGen::getGrid() const
{
    return grid;     
}
void BoardGen::setGrid(std::vector<std::vector<std::unique_ptr<Cell>>> newGrid)
{
    grid = std::move(newGrid);
    rows = static_cast<int>(grid.size());
    cols = static_cast<int>(grid[0].size());

    // przeliczenie bomb w nowej siatce
    bombs = 0;
    for (auto& r : grid)
        for (auto& c : r)
            if (c && c->isBomb()) ++bombs;
}
