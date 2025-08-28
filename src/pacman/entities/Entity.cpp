#include "pacman/entities/Entity.h"

void Entity::update(Board const& board)
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

Coordinate Entity::getPosition() const
{
    return mPos;
}

void Entity::setPosition(Coordinate const& newPos)
{
    mPos = newPos;
}

Coordinate Entity::getLastPosition() const
{
    return mLastPos;
}

void Entity::setLastPosition(Coordinate const& newLastPos)
{
    mLastPos = newLastPos;
}