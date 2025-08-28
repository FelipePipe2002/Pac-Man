#include "pacman/Entities/Player.h"

std::string Player::getSymbol() const
{
    switch (dir)
    {
    case Direction::Right:
        return "🌜";
    case Direction::Left:
        return "🌛";
    default:
        return "🌜";
    }
}

void Player::move(Board const& board)
{

    Coordinate newPos = mPos;

    switch (dir)
    {
    case Direction::Up:
        newPos.y--;
        break;
    case Direction::Down:
        newPos.y++;
        break;
    case Direction::Left:
        newPos.x--;
        break;
    case Direction::Right:
        newPos.x++;
        break;
    default:
        break;
    }

    if (board.isEnabled(newPos))
    {
        mLastPos = mPos;
        mPos = newPos;
    }
}

Direction Player::getDirection() const
{
    return dir;
}
void Player::setDirection(Direction newDir)
{
    dir = newDir;
}