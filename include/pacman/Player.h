#pragma once

#include "pacman/Board.h"
#include "pacman/Coordinate.h"
#include "pacman/Direction.h"

#include <string>

struct Player
{
    Coordinate pos{0, 0};
    Direction dir{Direction::None};

    Player(Coordinate const& startPos, Direction startDir) : pos(startPos), dir(startDir) {}

    [[nodiscard]] std::string getSymbol() const;

    void move(Board const& board);
};