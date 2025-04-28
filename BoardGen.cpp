// BoardGen.cpp
#include "BoardGen.h"

BoardGen::BoardGen(int rows, int cols, int bombs)
    : rows(rows), cols(cols), bombCount(bombs)
{
    board.resize(rows);
    for (int r = 0; r < rows; ++r) {
        board[r].resize(cols);
    }

    // Fill all cells with NormalCells (neighbor count = 0)
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            board[r][c] = std::make_unique<NormalCell>(0);
        }
    }

    placeBombs();
}


void BoardGen::placeBombs() {
    // Generate list of all possible positions
    std::vector<std::pair<int, int>> positions;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            positions.emplace_back(r, c);
        }
    }

    // Shuffle the positions
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(positions.begin(), positions.end(), gen);

    // Place bombs at the first bombCount positions
    for (int i = 0; i < bombCount; ++i) {
        int r = positions[i].first;
        int c = positions[i].second;

        board[r][c] = std::make_unique<BombCell>();

        // Update neighbor counts for 8 surrounding tiles
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr != 0 || dc != 0) { // Skip the bomb tile itself
                    incrementNeighbor(r + dr, c + dc);
                }
            }
        }
    }
}

void BoardGen::incrementNeighbor(int r, int c) {
    if (r >= 0 && r < rows && c >= 0 && c < cols) {
        if (!board[r][c]->isBomb()) {
            if (NormalCell* normal = dynamic_cast<NormalCell*>(board[r][c].get())) {
                normal->setNeighborCount(normal->getNeighborCount() + 1);
            }
        }
    }
}

Cell* BoardGen::getCell(int row, int col) const {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return board[row][col].get();
    }
    return nullptr;
}

int BoardGen::getRows() const {
    return rows;
}

int BoardGen::getCols() const {
    return cols;
}
