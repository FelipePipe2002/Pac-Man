#pragma once

#include "pacman/Ghost.h"
#include "pacman/Board.h"
#include "../../src/main.cpp"

struct MovementStrategy
{
    virtual ~MovementStrategy() = default;
    virtual void move(Ghost& ghost, Board const& board, Player const& player) = 0;
};


//TODO implement this
struct ChaseBlinky : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Player const& player) override {
    }
};

struct ScatterCornerTopRight : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Player const& player) override {}
};

struct FrightenedRandom : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Player const& player) override {}
};

struct EatenToHome : MovementStrategy
{
    void move(Ghost& ghost, Board const& board, Player const& player) override
    {
    }
};