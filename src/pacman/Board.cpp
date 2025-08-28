#include "pacman/utils/Color.h"
#include "pacman/utils/Coordinate.h"
#include "pacman/Entities/Player.h"
#include "pacman/Board.h"

#include <algorithm>
#include <iostream>

Board::Board(quicktype::mapJson mapJson)
{
    mWidth = Width{static_cast<int>(mapJson.get_width())};
    mHeight = Height{static_cast<int>(mapJson.get_height())};

    int const newSize = mWidth.raw() * mHeight.raw();
    mMatrix.clear();
    mMatrix.resize(newSize);

    std::vector<std::string> layoutMapString = mapJson.get_layout();

    if (layoutMapString.size() != mHeight.raw())
    {
        return;
    }

    std::map<std::string, Coordinate> mPortalsTemp;

    for (int y = 0; y < mHeight.raw(); y++)
    {
        if (layoutMapString[y].size() != mWidth.raw())
        {
            std::cerr << "Warning: layout row " << y << " has incorrect width " << layoutMapString[y].size()
                      << " (expected " << mWidth.raw() << ")" << std::endl;
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
                mPlayerStartingCoordinate = coord;
                setEnabled(coord, true);
                setPoint(coord, false);
            }
            else if (cell == 'H')
            {
                mGhostHomeCoordinate = coord;
                setEnabled(coord, true);
                setPoint(coord, false);
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
            else
            {
                std::cerr << "Warning: unknown map character '" << cell << "' at " << coord << std::endl;
                setEnabled(coord, true);
                setPoint(coord, false);
            }
        }
    }

    //Portals
    for (auto const& [id, coords] : mapJson.get_portals().get_data())
    {
        if (coords.size() != 2)
        {
            std::cerr << "Warning: portal " << id << " does not have 2 coordinates pairs\n";
            continue;
        }
        Coordinate c1(coords[0][0], coords[0][1]);
        Coordinate c2(coords[1][0], coords[1][1]);

        if (!isInBounds(c1) || !isInBounds(c2))
        {
            std::cerr << "Warning: portal " << id << " coordinates out of bounds\n";
            continue;
        }

        mPortals.emplace_back(c1, c2);
    }

    //Scatter points
    using coord = std::vector<int64_t>;
    std::map<std::string, coord> scatterPoints = mapJson.get_scatter_points();
    for (auto const& [key, value] : scatterPoints)
    {
        if (value.size() != 2)
        {
            std::cerr << "Warning: scatter point for " << key << " does not have 2 coordinates" << std::endl;
            continue;
        }
        Coordinate c(value[0], value[1]);

        if (!isEnabled(c))
        {
            std::cerr << "Warning: scatter point coordinates not on enabled tile for " << key << ": " << c << std::endl;
            continue;
        }

        if (!isInBounds(c))
        {
            std::cerr << "Warning: scatter point coordinates out of bounds for " << key << ": " << c << std::endl;
            continue;
        }

        setPellet(c, true);
        mScatterPoints[key] = c;
    }
}

std::string readId(std::string const& line, int& index)
{
    std::string id;
    int j = index + 1;
    while (j < line.size() && std::isdigit(line[j]))
    {
        id += line[j];
        ++j;
    }
    index = j - 1;
    return id.empty() ? "0" : id;
}

bool Board::isEnabled(Coordinate const& coord) const
{
    if (!isInBounds(coord))
    {
        return false;
    }
    return mMatrix[coordToPos(coord)].mEnabled;
}


bool Board::hasPoint(Coordinate const& coord) const
{
    if (!isInBounds(coord))
    {
        return false;
    }
    return mMatrix[coordToPos(coord)].mPoint;
}

bool Board::hasAPellet(Coordinate const& coord) const
{
    if (!isInBounds(coord))
    {
        return false;
    }
    return mMatrix[coordToPos(coord)].mPellet;

}

bool Board::hasFruit(Coordinate const& coord) const
{
    if (!isInBounds(coord))
    {
        return false;
    }
    return mMatrix[coordToPos(coord)].mFruit;
}

bool Board::allPointsCollected() const
{
    return std::all_of(mMatrix.begin(), mMatrix.end(), [](Tile const& t) { return !t.mPoint || !t.mEnabled; });
}


Coordinate Board::getPlayerStartingPoint() const
{
    return mPlayerStartingCoordinate;
}

Coordinate Board::getGhostHome() const
{
    return mGhostHomeCoordinate;
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

[[nodiscard]] std::optional<Coordinate> Board::getScatterPoint(std::string scatterPointId) const
{
    auto it = mScatterPoints.find(scatterPointId);
    if (it != mScatterPoints.end())
        return it->second;
    return std::nullopt;
}

void Board::setEnabled(Coordinate const& coord, bool enabled)
{
    if (!isInBounds(coord))
    {
        return;
    }
    mMatrix[coordToPos(coord)].mEnabled = enabled;
}

void Board::setPoint(Coordinate const& coord, bool enabled)
{
    if (!isInBounds(coord))
    {
        return;
    }
    mMatrix[coordToPos(coord)].mPoint = enabled;
}

void Board::setPellet(Coordinate const& coord, bool enabled)
{
    if (!isInBounds(coord))
    {
        return;
    }
    mMatrix[coordToPos(coord)].mPellet = enabled;
}

void Board::setFruit(Coordinate const& coord, bool enabled)
{
    if (!isInBounds(coord))
    {
        return;
    }
    mMatrix[coordToPos(coord)].mFruit = enabled;
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