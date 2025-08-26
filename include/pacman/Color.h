#pragma once
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

char const* const colorString(Color forColor)
{
    switch (forColor)
    {
    case Color::Red:
        return "\033[1;31m";
    case Color::Green:
        return "\033[1;32m";
    case Color::Yellow:
        return "\033[1;33m";
    case Color::Blue:
        return "\033[1;34m";
    case Color::Magenta:
        return "\033[1;35m";
    case Color::Cyan:
        return "\033[1;36m";
    case Color::White:
        return "\033[1;37m";
    default:
        return "InvalidColor";
    }
}

std::string applyColorWindows(Color color, std::string const& toMessage)
{
    static char const* const kResetColor = "\033[0m";
    return std::string{colorString(color)} + toMessage + kResetColor;
}

std::string applyColorPosix(Color color, std::string const& toMessage)
{
    static char const* const kResetColor = "\033[0m";
    return std::string{colorString(color)} + toMessage + kResetColor;
}

std::string applyColor(Color color, std::string const& toMessage)
{
#ifdef WIN32
    return applyColorWindows(color, toMessage);
#else
    return applyColorPosix(color, toMessage);
#endif
}