#pragma once

#include "pacman/Ghost.h"
#include "pacman/Board.h"

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