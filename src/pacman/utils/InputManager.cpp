#include "pacman/utils/InputManager.h"

#define NOMINMAX
#include <windows.h>

namespace InputManager
{
    std::optional<Direction> readDirectionBuffer()
    {
        if ((GetAsyncKeyState('W') & 0x8000) || (GetAsyncKeyState(VK_UP) & 0x8000))
            return Direction::Up;
        if ((GetAsyncKeyState('S') & 0x8000) || (GetAsyncKeyState(VK_DOWN) & 0x8000))
            return Direction::Down;
        if ((GetAsyncKeyState('A') & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000))
            return Direction::Left;
        if ((GetAsyncKeyState('D') & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000))
            return Direction::Right;

        return std::nullopt;
    }

    bool exitPressed()
    {
        return GetAsyncKeyState(VK_ESCAPE) & 0x8000;
    }

    bool enterPressed()
    {
        return GetAsyncKeyState(VK_RETURN) & 0x8000;
    }
} // namespace InputManager
