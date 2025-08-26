#pragma once

#include "pacman/Coordinate.h"
#include "pacman/StrongType.h"
#include "pacman/MapJson.h"
#include <functional>
#include <string>
#include <vector>
#include <optional>

using Width = StrongType<int, struct WidthTag>;
using Height = StrongType<int, struct HeightTag>;
using Rng = std::function<int(int, int)>;

class Board
{
public:
    Board(quicktype::mapJson mapJson);

    [[nodiscard]] bool isEnabled(Coordinate const& coord) const;
    [[nodiscard]] void setEnabled(Coordinate const& coord, bool enabled);

    [[nodiscard]] bool hasPoint(Coordinate const& coord) const;
    [[nodiscard]] void setPoint(Coordinate const& coord, bool enabled);
    [[nodiscard]] bool allPointsCollected() const;

    [[nodiscard]] Coordinate PlayerStratingPoint() const;

    void reset();

    [[nodiscard]] Width getWidth() const noexcept
    {
        return mWidth;
    };

    [[nodiscard]] Height getHeight() const noexcept
    {
        return mHeight;
    }

    [[nodiscard]] std::optional<Coordinate> teleportFrom(Coordinate coord) const;

private:
    struct Tile
    {
        bool mEnabled = true;
        bool mPoint = false;
    };

    Width mWidth{1};
    Height mHeight{1};
    std::vector<Tile> mMatrix;
    std::vector<std::pair<Coordinate, Coordinate>> mPortals;
    std::vector<Coordinate> mScatterPoints;

    Coordinate mPlayerStartingPoint;

    [[nodiscard]] int coordToPos(int x, int y) const;
    [[nodiscard]] int coordToPos(Coordinate coord) const;

    [[nodiscard]] bool isInBounds(Coordinate const& coord) const noexcept;

    static constexpr Width kDefaultSizeX{24};
    static constexpr Height kDefaultSizeY{24};
};