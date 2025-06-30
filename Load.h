#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include "Cell.h"
#include <filesystem>
#include "json.hpp"
#include <fstream>
#include <regex>
#include <concepts>


using Board = std::vector<std::vector<std::unique_ptr<Cell>>>;

template<typename T>
concept IsString = std::same_as<T, std::string>;

class Load
{
public:
    inline static std::string selectedFile;

    template <IsString T>
    static void save(const Board& board, const T& fileName)
    {
        using json = nlohmann::json;
        namespace fs = std::filesystem;

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

    static Board load(const std::string& fileName);

    static inline const std::filesystem::path kSaveDir{
        R"(C:\Users\jeren\OneDrive\Pulpit\minesweeper git\Minesweeper\saved)"
    };
    
};
