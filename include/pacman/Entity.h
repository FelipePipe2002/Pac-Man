#pragma once

#include "pacman/Board.h"
#include "pacman/Coordinate.h"

class Entity
{
public:
    Entity(Coordinate initPos) : mPos{initPos} {};

    void update(Board const& board)
    {
        move(board);

        auto optPortal = board.teleportFrom(mPos);
        if (optPortal.has_value())
        {
            mPos = optPortal.value();
            justTeleported = true;
        }
        else
        {
            justTeleported = false;
        }
    }

    virtual void move(Board const& board) = 0;

    Coordinate getPosition() const
    {
        return mPos;
    }
    void setPosition(Coordinate const& newPos)
    {
        mPos = newPos;
    }


protected:
    Coordinate mPos;
    bool justTeleported = false;
};