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
    LightWhite
};


// Solo declaraciones
char const* colorString(Color forColor);
std::string applyColorWindows(Color color, std::string const& toMessage);
std::string applyColorPosix(Color color, std::string const& toMessage);
std::string applyColor(Color color, std::string const& toMessage);
