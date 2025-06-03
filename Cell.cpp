#include "Cell.h"



void Cell::reveal() {
    if (!flagged)
        revealed = true;
}

bool Cell::isRevealed() const {
    return revealed;
}

void Cell::toggleFlag() {
    flagged = !flagged;
}

bool Cell::isFlagged() const {
    return flagged;
}



NormalCell::NormalCell(int neighbors) : neighborMines(neighbors) {}

bool NormalCell::isBomb() const {
    return false;
}

int NormalCell::getNeighborCount() const {
    return neighborMines;
}

void NormalCell::setNeighborCount(int count) {
    neighborMines = count;
}



bool BombCell::isBomb() const {
    return true;
}

int BombCell::getNeighborCount() const {
    return -1; 
}

void BombCell::setNeighborCount(int) {
    
}
