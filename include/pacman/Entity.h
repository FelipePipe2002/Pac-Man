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


        if (!optNewPos.has_value())
        {
            optNewPos = board.teleportFrom(mPos);
        }
        else
        {
            if (optNewPos)
            {
                mPos = optNewPos.value();
            }
            optNewPos = std::nullopt;
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

private:
    std::optional<Coordinate> optNewPos = std::nullopt;
};