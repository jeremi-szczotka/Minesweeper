#pragma once

#include <concepts>

// Concept dla komórek, które maj¹ s¹siaduj¹ce miny i nie s¹ bombami
template<typename T>
concept NormalCellConcept = requires(T cell) {
    { cell.setNeighborCount(0) } -> std::same_as<void>;
    { cell.getNeighborCount() } -> std::same_as<int>;
    { cell.isBomb() } -> std::same_as<bool>;
};

