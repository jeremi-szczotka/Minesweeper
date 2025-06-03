#pragma once

class Cell {
protected:
    bool revealed = false;
    bool flagged = false;

public:
    virtual ~Cell() = default;

    virtual bool isBomb() const = 0;
    virtual int getNeighborCount() const = 0;
    virtual void setNeighborCount(int count) = 0;

    virtual void reveal();
    virtual bool isRevealed() const;
    virtual void toggleFlag();
    virtual bool isFlagged() const;
};



class NormalCell : public Cell {
private:
    int neighborMines = 0;

public:
    NormalCell(int neighbors = 0);

    bool isBomb() const override;
    int getNeighborCount() const override;
    void setNeighborCount(int count) override;
};



class BombCell : public Cell {
public:
    bool isBomb() const override;
    int getNeighborCount() const override;
    void setNeighborCount(int count) override; // mo¿na zostawiæ pust¹ implementacjê
};
