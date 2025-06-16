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

    using Board = std::vector<std::vector<std::unique_ptr<Cell>>>;

class Load
{
    public:        
        static void save(const Board& board, const std::string& fileName);       
        static Board load(const std::string& fileName);
    private:       
        static inline const std::filesystem::path kSaveDir{ R"(C:\Users\jeren\OneDrive\Pulpit\minesweeper git\Minesweeper\saved)" };
};
