#pragma once
#include "pacman/Board.h"
#include "pacman/Player.h"
#include <optional>

class Ghost; // forward declaration

struct MovementStrategy
{
    virtual ~MovementStrategy() = default;
    virtual void move(Ghost& ghost, Board const& board, Entity* target) = 0;

protected:
    Direction findWay(Board const& board, Coordinate startPos, Coordinate targetPos, std::optional<Coordinate> lastPos);
};

struct ChaseBlinky : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Entity* target) override;
};

struct ScatterToScatterPoint : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Entity* target) override;
};

struct FrightenedRandom : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Entity* target) override;
};

struct EatenToHome : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Entity* target) override;
};