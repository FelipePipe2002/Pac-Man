#pragma once

#include "pacman/Board.h"
#include "pacman/utils/Coordinate.h"

class Entity
{
public:
    Entity(Coordinate initPos) : mPos{initPos} {};

    virtual void move(Board const& board) = 0;
    void update(Board const& board);

    [[nodiscard]] Coordinate getPosition() const;
    void setPosition(Coordinate const& newPos);

    [[nodiscard]] Coordinate getLastPosition() const;
    void setLastPosition(Coordinate const& newLastPos);

protected:
    Coordinate mPos;
    Coordinate mLastPos;

private:
    std::optional<Coordinate> optNewPos = std::nullopt;
};