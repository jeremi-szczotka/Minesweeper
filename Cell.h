// Cell.h
#pragma once
#include <memory>

class Cell {
protected:
    bool revealed = false;
    bool flagged = false;

public:
    virtual ~Cell() = default;

    virtual bool isBomb() const = 0;
    virtual int getNeighborCount() const = 0;

    void reveal();
    bool isRevealed() const;

    void toggleFlag();
    bool isFlagged() const;
};

class NormalCell : public Cell {
private:
    int neighborMines;

public:
    NormalCell(int neighbors = 0);

    bool isBomb() const override;
    int getNeighborCount() const override;

    void setNeighborCount(int count);
};

class BombCell : public Cell {
public:
    bool isBomb() const override;
    int getNeighborCount() const override;
};
