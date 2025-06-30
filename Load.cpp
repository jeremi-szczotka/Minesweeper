#include "Load.h"


using     json = nlohmann::json;



namespace fs = std::filesystem;
Board Load::load(const std::string& filename) {
    const fs::path fullPath = kSaveDir / filename;   
    std::ifstream  file(fullPath);                  
    if (!file)                                       
        throw std::runtime_error("Cannot open " + fullPath.string());

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


