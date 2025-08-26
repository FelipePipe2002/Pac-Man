#pragma  once

#include "pacman/Direction.h"
#include "pacman/Coordinate.h"

struct Player
{
    Coordinate pos = Coordinate(0, 0);
    Direction dir = Direction::None;

    std::string getSymbol()
    {
        if (dir == Direction::Right)
        {
            symbol = "🌜";
        }
        else if (dir == Direction::Left)
        {
            symbol = "🌛";
        }
        return symbol;
    }

    Coordinate move()
    {
        switch (dir)
        {
        case Direction::Up:
            pos.y--;
            break;
        case Direction::Down:
            pos.y++;
            break;
        case Direction::Left:
            pos.x--;
            break;
        case Direction::Right:
            pos.x++;
            break;
        default:
            break;
        }
        return pos;
    }

private:
    std::string symbol = "🌜";
};