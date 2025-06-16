#include "Load.h"


namespace fs = std::filesystem;
using     json = nlohmann::json;


void Load::save(const Board& board, const std::string& fileName)
{
    json jBoard;

    for (std::size_t r = 0; r < board.size(); ++r)
        for (std::size_t c = 0; c < board[r].size(); ++c)
        {
            json cell;
            cell["row"] = r;
            cell["col"] = c;
            cell["revealed"] = board[r][c]->isRevealed();
            cell["hasBomb"] = board[r][c]->isBomb();
            cell["neighborCount"] = board[r][c]->getNeighborCount();
            jBoard.push_back(std::move(cell));
        }

  
    constexpr char subject[] =
        R"(C:\Users\jeren\OneDrive\Pulpit\minesweeper git\Minesweeper\saved)";

    std::regex pattern(R"(C:\\Users\\.+\\OneDrive\\Pulpit\\minesweeper git\\Minesweeper\\saved)");

    if (std::regex_match(subject, pattern)) {
        std::cout << "Success: path matches!\n";
    }
    else {
        std::cout << "Failure: path does not match.\n";
    }


    fs::create_directories(kSaveDir);
        const fs::path fullPath = kSaveDir / fileName;

        std::ofstream out(fullPath);
        if (!out) {
            throw std::runtime_error("Cannot open " + fullPath.string());
        }

    out << jBoard.dump(4);
}

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
