#pragma once
#include <ostream>

struct Coordinate
{
    int x = 0;
    int y = 0;
};

inline std::ostream& operator<<(std::ostream& os, Coordinate const& c)
{
    return os << "(" << c.x << ", " << c.y << ")";
}

inline bool operator==(Coordinate const& lhs, Coordinate const& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator<(Coordinate const& lhs, Coordinate const& rhs)
{
    return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y);
}

inline int hashCoord(Coordinate c, int width)
{
    return c.y * width + c.x;
}

inline size_t hashCoord(Coordinate const &coord) noexcept
{
    size_t xHash = std::hash<int>()(coord.x);
    size_t yHash = std::hash<int>()(coord.y);
    return xHash ^ (yHash << 1);
}

namespace std
{
    template <>
    struct hash<Coordinate>
    {
        size_t operator()(Coordinate const& coord) const noexcept
        {
            return hashCoord(coord);
        }
    };
}