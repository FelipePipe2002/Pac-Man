#pragma once

#include "pacman/Board.h"
#include "pacman/Color.h"
#include "pacman/Coordinate.h"
#include "pacman/MovementStrategy.h"

enum class GhostState
{
    Chase,
    Scatter,
    Frightened,
    Eaten
};

struct Player;

class Ghost
{
public:
    Ghost(Color color, Coordinate coord) : mColor{color}, mPos{coord} {}
    ~Ghost() = default;

    void move(Board const& board, Player const& player);

    void setState(GhostState newState)
    {
        mState = newState;
    }
    [[nodiscard]] GhostState getState() const
    {
        return mState;
    }
    [[nodiscard]] Color getColor() const
    {
        return mColor;
    }
    [[nodiscard]] Coordinate getPos() const
    {
        return mPos;
    }
    void setPos(Coordinate coord)
    {
        mPos = coord;
    }
    [[nodiscard]] Coordinate getLastPos() const
    {
        return mLastPos;
    }

private:
    Color mColor;
    GhostState mState = GhostState::Scatter;
    Coordinate mPos;
    Coordinate mLastPos;

    std::unique_ptr<MovementStrategy> mChaseStrategy;
    std::unique_ptr<MovementStrategy> mScatterStrategy;
    std::unique_ptr<MovementStrategy> mFrightenedStrategy;
    std::unique_ptr<MovementStrategy> mEatenStrategy;
};
