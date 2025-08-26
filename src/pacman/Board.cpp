#include "pacman/Board.h"
#include "pacman/Coordinate.h"

#include <algorithm>
#include <iostream>

Board::Board(quicktype::mapJson mapJson)
{
    mWidth = Width{mapJson.get_width()};
    mHeight = Height{mapJson.get_height()};

    int const newSize = mWidth.raw() * mHeight.raw();
    mMatrix.clear();
    mMatrix.resize(newSize);
    reset();

    std::vector<std::string> layoutMapString = mapJson.get_layout();

    if (layoutMapString.size() != mHeight.raw())
    {
        return;
    }

    std::vector<std::pair<int, Coordinate>> portals;

    for (int y = 0; y < mHeight.raw(); y++)
    {
        if (layoutMapString[y].size() != mWidth.raw())
        {
            return;
        }

        for (int x = 0; x < mWidth.raw(); x++)
        {
            char cell = layoutMapString[y][x];
            Coordinate coord(x, y);

            if (cell == '#')
            {
                setEnabled(coord, false);
                setPoint(coord, false);
            }
            else if (cell == 'P')
            {
                mPlayerStartingPoint = coord;
                setEnabled(coord, true);
                setPoint(coord, false);
            }
            else if(cell == 'S')
            {
                mScatterPoints.push_back(coord);
                setEnabled(coord, true);
                setPoint(coord, true);
            }
            else if (cell == ' ')
            {
                setEnabled(coord, true);
                setPoint(coord, false);
            }
            else if (cell == '.')
            {
                setEnabled(coord, true);
                setPoint(coord, true);
            }
            else if (std::isdigit(cell))
            {
                int portalId = cell;
                int i = 0;
                for (i; i < portals.size(); i++)
                {
                    if (portals[i].first == portalId) //there is already a portal so be find the other one
                    {
                        mPortals.push_back({portals[i].second, coord});
                    }
                }
                if (i == portals.size()) // new portal
                {
                    portals.push_back({portalId, coord});
                }
                else //means that we found the second portal
                {
                    portals.erase(portals.begin() + i);
                }
                setEnabled(coord, true);
                setPoint(coord, false);
            }
            else
            {
                setEnabled(coord, true);
                setPoint(coord, true);
            }//Implement save ghost with the strategy then at the end generate them
        }
    }
    for (std::pair<Coordinate, Coordinate> p : mPortals)
    {
        std::cout << p.first << " y " << p.second << std::endl;
    }
}

bool Board::isEnabled(Coordinate const& coord) const
{
    if (!isInBounds(coord))
    {
        return false;
    }
    return mMatrix[coordToPos(coord)].mEnabled;
}

void Board::setEnabled(Coordinate const& coord, bool enabled)
{
    if (!isInBounds(coord))
    {
        return;
    }
    mMatrix[coordToPos(coord)].mEnabled = enabled;
}

bool Board::hasPoint(Coordinate const& coord) const
{
    if (!isInBounds(coord))
    {
        return false;
    }
    return mMatrix[coordToPos(coord)].mPoint;
}

void Board::setPoint(Coordinate const& coord, bool enabled)
{
    if (!isInBounds(coord))
    {
        return;
    }
    mMatrix[coordToPos(coord)].mPoint = enabled;
}

Coordinate Board::PlayerStratingPoint() const
{
    return mPlayerStartingPoint;
}

void Board::reset()
{
    for (auto& tile : mMatrix)
    {
        tile.mEnabled = true;
        tile.mPoint = true;
    }
}

bool Board::allPointsCollected() const
{
    return std::all_of(mMatrix.begin(), mMatrix.end(), [](Tile const& t) { return !t.mPoint || !t.mEnabled; });
}

std::optional<Coordinate> Board::teleportFrom(Coordinate coord) const 
{
    for (std::pair<Coordinate, Coordinate> p : mPortals)
    {
        if (p.first == coord)
        {
            return p.second;
        }
        else if (p.second == coord)
        {
            return p.first;
        }
    }
    return std::nullopt;
}
// ---------- PRIVATE ----------

int Board::coordToPos(int x, int y) const
{
    return y * mWidth.raw() + x;
}

int Board::coordToPos(Coordinate coord) const
{
    return coordToPos(coord.x, coord.y);
}

bool Board::isInBounds(Coordinate const& coord) const noexcept
{
    return coord.x >= 0 && coord.y >= 0 && coord.x < mWidth.raw() && coord.y < mHeight.raw();
}