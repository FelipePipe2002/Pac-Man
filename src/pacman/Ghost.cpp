#include "pacman/Ghost.h"
#include "pacman/Board.h"

void Ghost::move(Board const& board, Player const& player)
{
    Coordinate auxPos = mPos;
    switch (mState)
    {
    case GhostState::Chase:
        mChaseStrategy->move(*this, board, player);
        break;
    case GhostState::Scatter:
        mScatterStrategy->move(*this, board, player);
        break;
    case GhostState::Frightened:
        mFrightenedStrategy->move(*this, board, player);
        break;
    case GhostState::Eaten:
        mEatenStrategy->move(*this, board, player);
        break;
    default:
        break;
    }
    mLastPos = auxPos;

}