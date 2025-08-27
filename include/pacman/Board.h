#pragma once

#include "pacman/Coordinate.h"
#include "pacman/StrongType.h"
#include "pacman/json/MapJson.h"
#include <functional>
#include <string>
#include <vector>
#include <optional>

using Width = StrongType<int, struct WidthTag>;
using Height = StrongType<int, struct HeightTag>;
using Rng = std::function<int(int, int)>;

struct Player;

std::string readId(std::string const& line, int& index);

class Board
{
public:
    Board(quicktype::mapJson mapJson);

    [[nodiscard]] Width getWidth() const noexcept
    {
        return mWidth;
    }
    [[nodiscard]] Height getHeight() const noexcept
    {
        return mHeight;
    }

    [[nodiscard]] bool isEnabled(Coordinate const& coord) const;
    [[nodiscard]] bool hasPoint(Coordinate const& coord) const;
    [[nodiscard]] bool allPointsCollected() const;

    [[nodiscard]] Coordinate getPlayerStartingPoint() const;
    [[nodiscard]] Coordinate getGhostHome() const;


    [[nodiscard]] std::optional<Coordinate> teleportFrom(Coordinate coord) const;
    [[nodiscard]] std::optional<Coordinate> getScatterPoint(std::string scatterPointId) const;

    void setEnabled(Coordinate const& coord, bool enabled);
    void setPoint(Coordinate const& coord, bool enabled);

private:
    struct Tile
    {
        bool mEnabled = true;
        bool mPoint = false;
    };

    Width mWidth{1};
    Height mHeight{1};
    std::vector<Tile> mMatrix;

    //Game elements
    std::vector<std::pair<Coordinate, Coordinate>> mPortals;
    std::map<std::string, Coordinate> mScatterPoints;

    Coordinate mGhostHomeCoordinate;
    Coordinate mPlayerStartingCoordinate;

    [[nodiscard]] int coordToPos(int x, int y) const;
    [[nodiscard]] int coordToPos(Coordinate coord) const;
    [[nodiscard]] bool isInBounds(Coordinate const& coord) const noexcept;

    static constexpr Width kDefaultSizeX{24};
    static constexpr Height kDefaultSizeY{24};
};