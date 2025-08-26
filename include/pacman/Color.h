#pragma once
#include <string>

enum class Color
{
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White
};

// Solo declaraciones
char const* colorString(Color forColor);
std::string applyColorWindows(Color color, std::string const& toMessage);
std::string applyColorPosix(Color color, std::string const& toMessage);
std::string applyColor(Color color, std::string const& toMessage);
