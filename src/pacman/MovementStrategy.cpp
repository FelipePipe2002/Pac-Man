#include "pacman/MovementStrategy.h"
#include "pacman/Ghost.h"
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

void ScatterToScatterPoint::move(Ghost& ghost, Board const& board, Entity* target)
{
    Direction dir = findWay(board, ghost.getPosition(), ghost.getScatterPoint(), ghost.getLastPos());
    ghost.setPosition(applyDirection(ghost.getPosition(), dir));
}

void ChaseBlinky::move(Ghost& ghost, Board const& board, Entity* target)
{
    if (!target)
    {
        return;
    }
    Direction dir = findWay(board, ghost.getPosition(), target->getPosition(), ghost.getLastPos());
    ghost.setPosition(applyDirection(ghost.getPosition(), dir));
}

void FrightenedRandom::move(Ghost& ghost, Board const& board, Entity* target)
{
    // Implementación
}

void EatenToHome::move(Ghost& ghost, Board const& board, Entity* target)
{
    Direction dir = findWay(board, ghost.getPosition(), board.getGhostHome(), ghost.getLastPos());
    ghost.setPosition(applyDirection(ghost.getPosition(), dir));
}