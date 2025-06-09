#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Cell.h"

class Load {
    using Board = std::vector<std::vector<std::unique_ptr<Cell>>>;
public:
    static void save(const std::vector<std::vector<std::unique_ptr<Cell>>>& board, const std::string& filename);
    static Board load(const std::string& filename);
};
