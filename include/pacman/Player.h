#pragma once

#include "pacman/Board.h"
#include "pacman/Coordinate.h"
#include "pacman/Direction.h"
#include "pacman/Entity.h"
#include <string>

class Player : public Entity
{
public:
    Player(Coordinate const& startPos, Direction startDir) : Entity(startPos), dir(startDir) {}

    [[nodiscard]] std::string getSymbol() const;

    void move(Board const& board) override;

    Direction getDirection() const
    {
        return dir;
    }
    void setDirection(Direction newDir)
    {
        dir = newDir;
    }

private:
    Direction dir{Direction::None};
};
