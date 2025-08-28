#pragma once

#include "pacman/Entities/Ghost.h"
#include "pacman/Board.h"

Ghost::Ghost(Color color,
    Coordinate initPos,
    Coordinate scatterPoint,
    std::unique_ptr<MovementStrategy> chaseStrategy,
    Player* target)
: Entity(initPos),
  mColor{color},
  mOriginalColor{color},
  mScatterPoint{scatterPoint},
  mChaseStrategy{std::move(chaseStrategy)},
  mTarget{target}
{
    mScatterStrategy = std::make_unique<ScatterToScatterPoint>();
    mFrightenedStrategy = std::make_unique<FrightenedRandom>();
    mEatenStrategy = std::make_unique<EatenToHome>();
}

void Ghost::move(Board const& board)
{
    Coordinate auxPos = mPos;

    switch (mState)
    {
    case GhostState::Chase:
        mChaseStrategy->move(*this, board, mTarget);
        break;
    case GhostState::Scatter:
        mScatterStrategy->move(*this, board, mTarget);
        break;
    case GhostState::Frightened:
        mFrightenedStrategy->move(*this, board, mTarget);
        break;
    case GhostState::Eaten:
        mEatenStrategy->move(*this, board, mTarget);
        break;
    default:
        break;
    }
    mLastPos = auxPos;
}


GhostState Ghost::getState() const
{
    return mState;
}

Color Ghost::getColor() const
{
    return mColor;
}

Color Ghost::getOriginalColor() const
{
    return mOriginalColor;
}

Coordinate Ghost::getScatterPoint() const
{
    return mScatterPoint;
}

void Ghost::setState(GhostState newState)
{
    mState = newState;
}

void Ghost::setColor(Color color)
{
    mColor = color;
}

void Ghost::setTarget(Player& target)
{
    mTarget = &target;
}