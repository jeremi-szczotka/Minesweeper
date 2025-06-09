#include "Load.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

void Load::save(const std::vector<std::vector<std::unique_ptr<Cell>>>& board, const std::string& filename) {
    json jBoard;

    for (size_t i = 0; i < board.size(); ++i) {
        for (size_t j = 0; j < board[i].size(); ++j) {
            json cell;
            cell["row"] = i;
            cell["col"] = j;
            cell["revealed"] = board[i][j]->isRevealed();
            cell["hasBomb"] = board[i][j]->isBomb(); // ← używamy isBomb()
            cell["neighborCount"] = board[i][j]->getNeighborCount(); // ← oryginalna metoda
            jBoard.push_back(cell);
        }
    }

    std::ofstream file(filename);
    file << jBoard.dump(4);
}

std::vector<std::vector<std::unique_ptr<Cell>>> Load::load(const std::string& filename) {
    std::ifstream file(filename);
    json jBoard;
    file >> jBoard;

    size_t maxRow = 0, maxCol = 0;
    for (const auto& cell : jBoard) {
        maxRow = std::max(maxRow, static_cast<size_t>(cell["row"]));
        maxCol = std::max(maxCol, static_cast<size_t>(cell["col"]));
    }


    std::vector<std::vector<std::unique_ptr<Cell>>> board(maxRow + 1);

    for (auto& row : board) {
        row.reserve(maxCol + 1);
    }

    for (const auto& cell : jBoard) {
        int i = cell["row"];
        int j = cell["col"];
        bool hasBomb = cell["hasBomb"];
        bool revealed = cell["revealed"];
        int neighbors = cell["neighborCount"];

        std::unique_ptr<Cell> ptr;

        if (hasBomb) {
            ptr = std::make_unique<BombCell>();
        }
        else {
            auto normal = std::make_unique<NormalCell>();
            normal->setNeighborCount(neighbors);
            ptr = std::move(normal);
        }

        if (revealed) {
            ptr->reveal();
        }

        
        if (board[i].size() <= j)
            board[i].resize(j + 1);

        board[i][j] = std::move(ptr);
    }


    return board;
}
