#pragma once

#include <concepts>

// Concept dla kom�rek, kt�re maj� s�siaduj�ce miny i nie s� bombami
template<typename T>
concept NormalCellConcept = requires(T cell) {
    { cell.setNeighborCount(0) } -> std::same_as<void>;
    { cell.getNeighborCount() } -> std::same_as<int>;
    { cell.isBomb() } -> std::same_as<bool>;
};

