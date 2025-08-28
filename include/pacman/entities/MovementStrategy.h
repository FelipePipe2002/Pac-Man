#pragma once
#include "pacman/Board.h"
#include "pacman/Entities/Player.h"
#include "pacman/utils/Direction.h"
#include <optional>
#include <iostream>

class Ghost;

struct MovementStrategy
{
    virtual ~MovementStrategy() = default;
    virtual void move(Ghost& ghost, Board const& board, Player* target) = 0;

protected:
    Direction findWay(Board const& board, Coordinate startPos, Coordinate targetPos, std::optional<Coordinate> lastPos);
};

struct ChaseBlinky : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Player* target) override;
};

struct ChasePinky : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Player* target) override;
};

struct ChaseClide : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Player* target) override;
};

struct ChaseInki : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Player* target) override;
};


struct ScatterToScatterPoint : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Player* target) override;
};

struct FrightenedRandom : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Player* target) override;
};

struct EatenToHome : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Player* target) override;
};

inline std::unique_ptr<MovementStrategy> strategyFromString(std::string const& s)
{
    if (s.empty())
        return nullptr;

    switch (s[0])
    {
    case 'B':
        return std::make_unique<ChaseBlinky>();
    case 'P':
        return std::make_unique<ChasePinky>();
    case 'C':
        return std::make_unique<ChaseClide>();
    case 'I':
        return std::make_unique<ChaseInki>();
    default:
        return nullptr;
    }
}