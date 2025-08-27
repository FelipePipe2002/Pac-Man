#include "pacman/Color.h"

char const* colorString(Color forColor)
{
    switch (forColor)
    {
    case Color::Default:
        return "\033[0m";

    // Brillantes
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

    // Oscuros / apagados
    case Color::DarkRed:
        return "\033[0;31m";
    case Color::DarkGreen:
        return "\033[0;32m";
    case Color::DarkYellow:
        return "\033[0;33m";
    case Color::DarkBlue:
        return "\033[0;34m";
    case Color::DarkMagenta:
        return "\033[0;35m";
    case Color::DarkCyan:
        return "\033[0;36m";
    case Color::DarkWhite:
        return "\033[0;37m";

    // Pasteles / light
    case Color::LightRed:
        return "\033[1;91m";
    case Color::LightGreen:
        return "\033[1;92m";
    case Color::LightYellow:
        return "\033[1;93m";
    case Color::LightBlue:
        return "\033[1;94m";
    case Color::LightMagenta:
        return "\033[1;95m";
    case Color::LightCyan:
        return "\033[1;96m";
    case Color::LightWhite:
        return "\033[1;97m";

    default:
        return "\033[0m";
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

