#include "pacman/Board.h"
#include "pacman/Coordinate.h"
#include "pacman/MapJson.h"
#include "pacman/Color.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#define NOMINMAX
#include <windows.h>

//===========================
//          Player
//===========================

struct Player
{
    Coordinate pos;
    std::string symbol = "🌜";
};

void tryChangeDirection(char& actual, char candidate, Coordinate const& pos, Board const& board)
{
    Coordinate newPos = pos;
    switch (candidate)
    {
    case 'W':
    {
        newPos.y--;
        if (board.isEnabled(newPos))
        {
            actual = 'W';
        }
        break;
    }
    case 'S':
    {
        newPos.y++;
        if (board.isEnabled(newPos))
        {
            actual = 'S';
        }
        break;
    }
    case 'A':
    {
        newPos.x--;
        if (board.isEnabled(newPos))
        {
            actual = 'A';
        }
        break;
    }
    case 'D':
    {
        newPos.x++;
        if (board.isEnabled(newPos))
        {
            actual = 'D';
        }
        break;
    }
    default:
        break;
    }
}

Coordinate nextPosition(Coordinate pos, char dir)
{
    switch (dir)
    {
    case 'W':
    {
        pos.y--;
        break;
    }
    case 'S':
    {
        pos.y++;
        break;
    }
    case 'A':
    {
        pos.x--;
        break;
    }
    case 'D':
    {
        pos.x++;
        break;
    }
    }
    return pos;
}

//===========================
//          Utils
//===========================
static void moveCursorHome()
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
}

static void drawGrid(Player player, Board const& board)
{
    int width = board.getWidth().raw();
    int height = board.getHeight().raw();

    std::cout << "ESC: quit | WASD: move\n\n";

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Coordinate actualPos(x, y);
            std::string symbol;
            if (player.pos == actualPos)
                symbol = applyColor(Color::Yellow, player.symbol);
            else if (!board.isEnabled(actualPos))
                symbol = applyColor(Color::Blue, "██");
            else if (board.hasPoint(actualPos))
                symbol = applyColor(Color::White, ". ");
            else
                symbol = "  ";

            if (actualPos == Coordinate(1, 1))
                symbol = applyColor(Color::Red, "👻");

            std::cout << symbol;
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

//===========================
//          Main
//===========================
int main()
{
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    using clock = std::chrono::steady_clock;
    constexpr auto kFrame = std::chrono::milliseconds(100);

    // HIDE CURSOR
    CONSOLE_CURSOR_INFO ci{25, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);

    // FRAME RATE INIT
    auto lastFrameStart = clock::now();
    auto nextFrame = lastFrameStart;
    auto fpsTick = lastFrameStart;



    // GET MAP
    std::ifstream map_file("../assets/map1.json");
    if (!map_file.is_open())
    {
        std::cout << "No se pudo abrir el archivo de mapa\n";
        return 1;
    }
    nlohmann::json j;
    map_file >> j;
    quicktype::mapJson map_data = j.get<quicktype::mapJson>();


    // GAME
    Board board = Board(map_data);
    Player player;
    player.pos = board.PlayerStratingPoint();

    char actual = ' ';
    char lastPressed = ' ';
    std::optional<Coordinate> optNewPos = std::nullopt;
    bool justTeleported = false;
    while (true)
    {
        auto const now = clock::now();
        lastFrameStart = now;

        // INPUT
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
        {
            moveCursorHome();
            ci.bVisible = TRUE;
            SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
            std::cout << "ESC pressed. Exiting...\n";
            break;
        }

        // DIRECTION
        if (GetAsyncKeyState('W') & 0x8000)
        {
            lastPressed = 'W';
        }
        else if (GetAsyncKeyState('S') & 0x8000)
        {
            lastPressed = 'S';
        }
        else if (GetAsyncKeyState('A') & 0x8000)
        {
            lastPressed = 'A';
        }
        else if (GetAsyncKeyState('D') & 0x8000)
        {
            lastPressed = 'D';
        }

        // BUFFER NEW MOVEMENT
        tryChangeDirection(actual, lastPressed, player.pos, board);

        if (actual == 'D')
        {
            player.symbol = "🌜";
        }
        else if (actual == 'A')
        {
            player.symbol = "🌛";
        }

        // MOVEMENT
        if (justTeleported)
        {
            justTeleported = false;
        }
        else
        {
            Coordinate newPos = nextPosition(player.pos, actual);

            if (board.isEnabled(newPos))
            {
                player.pos = newPos;
            }
        }

        if (board.hasPoint(player.pos))
        {
            board.setPoint(player.pos, false);
        }
        // CHECK FOR WIN
        if (board.allPointsCollected())
        {
            moveCursorHome();
            ci.bVisible = TRUE;
            SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
            std::cout << "¡Ganaste!\n";
            break;
        }

        // RENDER
        moveCursorHome();
        drawGrid(player, board);

        if (!optNewPos.has_value())
        {
            auto optPortal = board.teleportFrom(player.pos);
            if (optPortal.has_value())
            {
                player.pos = optPortal.value();
                justTeleported = true;
            }
            optNewPos = optPortal;
        }
        else
        {
            optNewPos = std::nullopt;
        }

        nextFrame += kFrame;
        if (nextFrame < now)
            nextFrame = now + kFrame;
        std::this_thread::sleep_until(nextFrame);
    }
}
