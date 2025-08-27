#pragma once

#include "pacman/Board.h"
#include "pacman/Color.h"
#include "pacman/Coordinate.h"
#include "pacman/Entity.h"
#include "pacman/MovementStrategy.h"
enum class GhostState
{
    Chase,
    Scatter,
    Frightened,
    Eaten
};

struct Player;

class Ghost : public Entity
{
public:
    Ghost(Color color,
        Coordinate initPos,
        Coordinate scatterPoint,
        std::unique_ptr<MovementStrategy> chaseStrategy,
        Player* target)
    : Entity(initPos), mColor{color}, mScatterPoint{scatterPoint}, mChaseStrategy{std::move(chaseStrategy)}, mTarget{target}
    {
        mScatterStrategy = std::make_unique<ScatterToScatterPoint>();
        mFrightenedStrategy = std::make_unique<FrightenedRandom>();
        mEatenStrategy = std::make_unique<EatenToHome>();
    }
    ~Ghost() = default;

    void move(Board const& board) override;

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

    void setColor(Color color)
    {
        mColor = color;
    }

    [[nodiscard]] Coordinate getLastPos() const
    {
        return mLastPos;
    }
    [[nodiscard]] Coordinate getScatterPoint() const
    {
        return mScatterPoint;
    }

private:
    Color mColor;
    GhostState mState = GhostState::Chase;
    Coordinate mScatterPoint;
    Coordinate mLastPos;
    Player* mTarget;

    std::unique_ptr<MovementStrategy> mChaseStrategy;
    std::unique_ptr<MovementStrategy> mScatterStrategy;
    std::unique_ptr<MovementStrategy> mFrightenedStrategy;
    std::unique_ptr<MovementStrategy> mEatenStrategy;
};
