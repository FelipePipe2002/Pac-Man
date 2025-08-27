#include "pacman/MovementStrategy.h"
#include "pacman/Ghost.h"

#include <cstdlib>
#include <math.h>
#include <queue>
#include <unordered_map>

Direction MovementStrategy::findWay(
    Board const& board, Coordinate startPos, Coordinate targetPos, std::optional<Coordinate> lastPos)
{
    std::vector<std::pair<Direction, Coordinate>> moves = {
        {Direction::Up, {0, -1}}, {Direction::Down, {0, 1}}, {Direction::Left, {-1, 0}}, {Direction::Right, {1, 0}}};

    std::queue<Coordinate> q;
    std::unordered_map<int, Coordinate> parent;
    std::unordered_map<int, Direction> firstMove;

    q.push(startPos);
    parent[hashCoord(startPos)] = startPos;

    while (!q.empty())
    {
        Coordinate current = q.front();
        q.pop();

        if (auto optPortal = board.teleportFrom(current))
        {
            Coordinate portalPos = *optPortal;
            int keyPortal = hashCoord(portalPos);

            if (board.isEnabled(portalPos) && (!lastPos || portalPos != *lastPos)
                && parent.find(keyPortal) == parent.end())
            {
                parent[keyPortal] = current;
                if (current == startPos)
                    firstMove[keyPortal] = Direction::None;
                else
                    firstMove[keyPortal] = firstMove[hashCoord(current)];

                if (portalPos == targetPos)
                    return firstMove[keyPortal];

                q.push(portalPos);
            }
        }

        for (auto [dir, delta] : moves)
        {
            Coordinate next{current.x + delta.x, current.y + delta.y};
            int key = hashCoord(next);

            if (!board.isEnabled(next) || (lastPos && next == *lastPos) || parent.find(key) != parent.end())
                continue;

            parent[key] = current;
            if (current == startPos)
                firstMove[key] = dir;
            else
                firstMove[key] = firstMove[hashCoord(current)];

            if (next == targetPos)
                return firstMove[key];

            q.push(next);
        }
    }

    for (auto [dir, delta] : moves)
    {
        Coordinate next{startPos.x + delta.x, startPos.y + delta.y};
        if (board.isEnabled(next) && (!lastPos || next != *lastPos))
            return dir;
    }

    if (lastPos)
    {
        if (lastPos->x == startPos.x && lastPos->y == startPos.y)
            return Direction::None;

        if (lastPos->x < startPos.x)
            return Direction::Left;
        if (lastPos->x > startPos.x)
            return Direction::Right;
        if (lastPos->y < startPos.y)
            return Direction::Up;
        if (lastPos->y > startPos.y)
            return Direction::Down;
    }

    return Direction::None;
}

Coordinate applyDirection(Coordinate pos, Direction dir)
{
    switch (dir)
    {
    case Direction::Up:
        return {pos.x, pos.y - 1};
    case Direction::Down:
        return {pos.x, pos.y + 1};
    case Direction::Left:
        return {pos.x - 1, pos.y};
    case Direction::Right:
        return {pos.x + 1, pos.y};
    default:
        return pos;
    }
}


// CHASE TYPES OF MOVEMENT
void ChaseBlinky::move(Ghost& ghost, Board const& board, Player* target)
{
    if (!target)
    {
        return;
    }
    Direction dir = findWay(board, ghost.getPosition(), target->getPosition(), ghost.getLastPos());
    ghost.setPosition(applyDirection(ghost.getPosition(), dir));
}

void ChasePinky::move(Ghost& ghost, Board const& board, Player* target)
{
    if (!target)
    {
        return;
    }
    Coordinate targetPos = target->getPosition();
    switch (target->getDirection())
    {
    case Direction::Up:
    {
        targetPos.y -= 2;
    }
    case Direction::Down:
    {
        targetPos.y += 2;
    }
    case Direction::Left:
    {
        targetPos.x -= 2;
    }
    case Direction::Right:
    {
        targetPos.x += 2;
    }
    }

    Direction dir = findWay(board, ghost.getPosition(), targetPos, ghost.getLastPos());
    ghost.setPosition(applyDirection(ghost.getPosition(), dir));
}

void ChaseClide::move(Ghost& ghost, Board const& board, Player* target)
{
    if (!target)
    {
        return;
    }
    Coordinate targetPos = target->getPosition();
    Coordinate ghostPos = ghost.getPosition();
    Direction dir = Direction::None;
    int dx = targetPos.x - ghostPos.x;
    int dy = targetPos.y - ghostPos.y;
    int dist2 = dx * dx + dy * dy;
    if (dist2 >= 64)
        dir = findWay(board, ghost.getPosition(), targetPos, ghost.getLastPos());
    else
        dir = findWay(board, ghost.getPosition(), ghost.getScatterPoint(), ghost.getLastPos());

    ghost.setPosition(applyDirection(ghost.getPosition(), dir));
}

// GENERAL MOVEMENT
void ScatterToScatterPoint::move(Ghost& ghost, Board const& board, Player* target)
{
    Direction dir = findWay(board, ghost.getPosition(), ghost.getScatterPoint(), ghost.getLastPos());
    ghost.setPosition(applyDirection(ghost.getPosition(), dir));
}

void FrightenedRandom::move(Ghost& ghost, Board const& board, Player* target)
{
    ghost.setColor(Color::DarkBlue);

    Direction randomDir = Direction::None;
    Coordinate newPos;
    do
    {
        int intRandomDir = rand() % 4;

        switch (intRandomDir)
        {
        case 0:
            randomDir = Direction::Up;
            break;
        case 1:
            randomDir = Direction::Down;
            break;
        case 2:
            randomDir = Direction::Left;
            break;
        case 3:
            randomDir = Direction::Right;
            break;
        default:
            randomDir = Direction::None;
            break;
        }

        newPos = applyDirection(ghost.getPosition(), randomDir);
    } while (newPos == ghost.getLastPos());


    if (board.isEnabled(newPos))
    {
        ghost.setPosition(newPos);
    }
}

void EatenToHome::move(Ghost& ghost, Board const& board, Player* target)
{
    Direction dir = findWay(board, ghost.getPosition(), board.getGhostHome(), ghost.getLastPos());
    ghost.setPosition(applyDirection(ghost.getPosition(), dir));
}