#include "pacman/utils/Console.h"

#define NOMINMAX
#include <windows.h>

namespace Console
{
    void initConsole()
    {
        SetConsoleOutputCP(CP_UTF8);
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
        CONSOLE_CURSOR_INFO ci{25, FALSE};
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
    }

    void moveCursorHome()
    {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
    }
} // namespace Console
