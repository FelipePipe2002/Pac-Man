#include "pacman/Board.h"
#include "pacman/Color.h"
#include "pacman/Coordinate.h"
#include "pacman/Direction.h"
#include "pacman/Player.h"
#include "pacman/json/MapJson.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#define NOMINMAX
#include <windows.h>

//===========================
//          Struct
//===========================

struct InputManager
{
    static std::optional<Direction> readDirectionBuffer()
    {
        if (GetAsyncKeyState('W') & 0x8000)
            return Direction::Up;
        if (GetAsyncKeyState('S') & 0x8000)
            return Direction::Down;
        if (GetAsyncKeyState('A') & 0x8000)
            return Direction::Left;
        if (GetAsyncKeyState('D') & 0x8000)
            return Direction::Right;
        return std::nullopt;
    }
    static bool exitPressed()
    {
        return GetAsyncKeyState(VK_ESCAPE) & 0x8000;
    }
};

//===========================
//          Utils
//===========================
Board loadMap(std::string const& filename)
{
    std::ifstream map_file(filename);
    if (!map_file.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo de mapa");
    }
    nlohmann::json j;
    map_file >> j;
    quicktype::mapJson map_data = j.get<quicktype::mapJson>();
    return Board(map_data);
}

static void moveCursorHome()
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
}

static void drawGrid(Player const& player, Board const& board)
{
    std::ostringstream frame;
    frame << "ESC: quit | WASD: move\n\n";

    int width = board.getWidth().raw();
    int height = board.getHeight().raw();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Coordinate actualPos(x, y);
            std::string symbol;
            if (player.pos == actualPos)
                symbol = applyColor(Color::Yellow, player.getSymbol());
            else if (!board.isEnabled(actualPos))
                symbol = applyColor(Color::Blue, "██");
            else if (board.hasPoint(actualPos))
                symbol = applyColor(Color::White, ". ");
            else
                symbol = "  ";

            if (actualPos == Coordinate(1, 1))
                symbol = applyColor(Color::Red, "👻");

            frame << symbol;
        }
        frame << "\n";
    }
    frame << "\n";

    std::cout << frame.str();
}

void tryChangeDirection(Player& player, Direction dirBuffer, Board const& board)
{
    Coordinate newPos = player.pos;
    switch (dirBuffer)
    {
        case Direction::Up:
            newPos.y--;
            break;
        case Direction::Down:
            newPos.y++;
            break;
        case Direction::Left:
            newPos.x--;
            break;
        case Direction::Right:
            newPos.x++;
            break;
        default:
            return;
    }

    if (board.isEnabled(newPos))
    {
        player.dir = dirBuffer;
    }
}

//===========================
//          Main
//===========================
int main()
{
    //Init Console
    SetConsoleOutputCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    CONSOLE_CURSOR_INFO ci{25, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);

    using clock = std::chrono::steady_clock;
    constexpr auto kFrame = std::chrono::milliseconds(100);
    

    // GAME
    Board board = loadMap("../assets/map1.json");
    Player player(board.getPlayerStartingPoint(), Direction::None);
    Direction dirBuffer = Direction::None;
    std::optional<Coordinate> optNewPos = std::nullopt;
    bool justTeleported = false;

    while (true)
    {
        auto const now = clock::now();

        // EXIT
        if (InputManager::exitPressed())
        {
            break;
        }

        // DIRECTION BUFFER
        if (auto optDirBuffer = InputManager::readDirectionBuffer())
        {
            dirBuffer = optDirBuffer.value();
        }
        tryChangeDirection(player, dirBuffer, board);


        // MOVEMENT
        if (justTeleported) // Just to show where the player is after tp
        {
            justTeleported = false;
        }
        else
        {
            player.move(board);
        }

        // CHECK FOR POINT
        if (board.hasPoint(player.pos))
        {
            board.setPoint(player.pos, false);
        }

        // CHECK FOR WIN
        if (board.allPointsCollected())
        {
            std::cout << "¡Ganaste!\n";
            break;
        }

        // RENDER
        moveCursorHome();
        drawGrid(player, board);


        // TELEPORT
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

        // FRAME LIMITER
        auto nextFrame = now + kFrame;
        std::this_thread::sleep_until(nextFrame);
    }
}
