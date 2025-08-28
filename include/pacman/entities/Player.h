#pragma once

#include "pacman/Board.h"
#include "pacman/utils/Coordinate.h"
#include "pacman/utils/Direction.h"
#include "pacman/Entities/Entity.h"
#include <string>

class Player : public Entity
{
public:
    Player(Coordinate const& startPos, Direction startDir) : Entity(startPos), dir(startDir) {}

    [[nodiscard]] std::string getSymbol() const;

    void move(Board const& board) override;

    Direction getDirection() const;
    void setDirection(Direction newDir);

private:
    Direction dir{Direction::None};
};
