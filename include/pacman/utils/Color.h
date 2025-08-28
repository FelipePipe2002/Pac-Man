#pragma once
#include <string>

//CREDIT TO: Hilen for providing this thing
enum class Color
{
    Default,
    // Colores brillantes
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,

    // Colores oscuros / apagados
    DarkRed,
    DarkGreen,
    DarkYellow,
    DarkBlue,
    DarkMagenta,
    DarkCyan,
    DarkWhite,

    // Colores intermedios / pasteles
    LightRed,
    LightGreen,
    LightYellow,
    LightBlue,
    LightMagenta,
    LightCyan,
    LightWhite,

    Black
};


// Solo declaraciones
char const* colorString(Color forColor);
std::string applyColorWindows(Color color, std::string const& toMessage);
std::string applyColorPosix(Color color, std::string const& toMessage);
std::string applyColor(Color color, std::string const& toMessage);
inline Color stringToColor(std::string const& str)
{
    if (str.size() < 2)
        return Color::Default;

    char brightness = str[0]; // N, D o L
    char base = str[1];       // R, G, Y, B, M, C, W

    switch (brightness)
    {
    case 'N': // Normal
        switch (base)
        {
        case 'R':
            return Color::Red;
        case 'G':
            return Color::Green;
        case 'Y':
            return Color::Yellow;
        case 'B':
            return Color::Blue;
        case 'M':
            return Color::Magenta;
        case 'C':
            return Color::Cyan;
        case 'W':
            return Color::White;
        default:
            return Color::Default;
        }
    case 'D': // Dark
        switch (base)
        {
        case 'R':
            return Color::DarkRed;
        case 'G':
            return Color::DarkGreen;
        case 'Y':
            return Color::DarkYellow;
        case 'B':
            return Color::DarkBlue;
        case 'M':
            return Color::DarkMagenta;
        case 'C':
            return Color::DarkCyan;
        case 'W':
            return Color::DarkWhite;
        default:
            return Color::Default;
        }
    case 'L': // Light
        switch (base)
        {
        case 'R':
            return Color::LightRed;
        case 'G':
            return Color::LightGreen;
        case 'Y':
            return Color::LightYellow;
        case 'B':
            return Color::LightBlue;
        case 'M':
            return Color::LightMagenta;
        case 'C':
            return Color::LightCyan;
        case 'W':
            return Color::LightWhite;
        default:
            return Color::Default;
        }
    default:
        return Color::Default;
    }
}