#include "pacman/Entities/MovementStrategy.h"
#include "pacman/Entities/Ghost.h"
#include "pacman/entities/Player.h"

#include <cstdlib>
#include <queue>
#include <random>
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
    Direction dir = findWay(board, ghost.getPosition(), target->getPosition(), ghost.getLastPosition());
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
        targetPos.y -= 2;
        break;
    case Direction::Down:
        targetPos.y += 2;
        break;
    case Direction::Left:
        targetPos.x -= 2;
        break;
    case Direction::Right:
        targetPos.x += 2;
        break;
    default:
        break;
    }

    // If the tile ahead is a wall or out of bounds, chase the player directly
    if (!board.isEnabled(targetPos))
    {
        targetPos = target->getPosition();
    }

    Direction dir = findWay(board, ghost.getPosition(), targetPos, ghost.getLastPosition());
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
        dir = findWay(board, ghost.getPosition(), targetPos, ghost.getLastPosition());
    else
        dir = findWay(board, ghost.getPosition(), ghost.getScatterPoint(), ghost.getLastPosition());

    ghost.setPosition(applyDirection(ghost.getPosition(), dir));
}

void ChaseInki::move(Ghost& ghost, Board const& board, Player* target)
{
    //The friends we made along the way
}

void ChasePortal::move(Ghost& ghost, Board const& board, Player* target)
{
    if (!target)
    {
        return;
    }

    Coordinate playerPos = target->getPosition();
    auto const& portals = board.getPortals();

    std::optional<std::pair<Coordinate, Coordinate>> closestPortal;
    int minDist2 = 1000;

    for (auto const& p : portals)
    {
        int dx1 = playerPos.x - p.first.x;
        int dy1 = playerPos.y - p.first.y;
        int dist1 = dx1 * dx1 + dy1 * dy1;

        int dx2 = playerPos.x - p.second.x;
        int dy2 = playerPos.y - p.second.y;
        int dist2 = dx2 * dx2 + dy2 * dy2;

        if (dist1 <= 25 && dist1 < minDist2)
        {
            minDist2 = dist1;
            closestPortal = p;
        }
        if (dist2 <= 25 && dist2 < minDist2)
        {
            minDist2 = dist2;
            closestPortal = p;
        }
    }

    if (closestPortal.has_value())
    {
        Coordinate targetCoord;
        if (playerPos == closestPortal->first)
            targetCoord = closestPortal->second;
        else
            targetCoord = closestPortal->first;

        Direction dir = findWay(board, ghost.getPosition(), targetCoord, ghost.getLastPosition());
        ghost.setPosition(applyDirection(ghost.getPosition(), dir));
    }
    else
    {
        Direction dir = findWay(board, ghost.getPosition(), ghost.getScatterPoint(), ghost.getLastPosition());
        ghost.setPosition(applyDirection(ghost.getPosition(), dir));
    }
}

// GENERAL MOVEMENT
void ScatterToScatterPoint::move(Ghost& ghost, Board const& board, Player* target)
{
    Direction dir = findWay(board, ghost.getPosition(), ghost.getScatterPoint(), ghost.getLastPosition());
    ghost.setPosition(applyDirection(ghost.getPosition(), dir));
}

void FrightenedRandom::move(Ghost& ghost, Board const& board, Player* target)
{
    ghost.setColor(Color::DarkBlue);

    std::vector<Direction> dirs = {Direction::Up, Direction::Down, Direction::Left, Direction::Right};
    std::shuffle(dirs.begin(), dirs.end(), std::mt19937{std::random_device{}()});

    for (auto dir : dirs)
    {
        Coordinate newPos = applyDirection(ghost.getPosition(), dir);
        if (board.isEnabled(newPos) && newPos != ghost.getLastPosition())
        {
            ghost.setPosition(newPos);
            return;
        }
    }
}

void EatenToHome::move(Ghost& ghost, Board const& board, Player* target)
{
    Direction dir = findWay(board, ghost.getPosition(), board.getGhostHome(), ghost.getLastPosition());
    ghost.setPosition(applyDirection(ghost.getPosition(), dir));
}