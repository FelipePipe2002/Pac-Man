#include "pacman/Board.h"
#include "pacman/Color.h"
#include "pacman/Coordinate.h"
#include "pacman/Direction.h"
#include "pacman/Ghost.h"
#include "pacman/MovementStrategy.h"
#include "pacman/Player.h"
#include "pacman/json/MapJson.h"

#include <chrono>
#include <cstdlib>
#include <ctime>
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

std::vector<std::unique_ptr<Ghost>> loadGhost(std::string const& filename, Board const& board, Player* player)
{
    std::ifstream map_file(filename);
    if (!map_file.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo de mapa");
    }

    nlohmann::json j;
    map_file >> j;
    quicktype::mapJson map_data = j.get<quicktype::mapJson>();

    std::vector<std::unique_ptr<Ghost>> ghosts; // solo este

    for (quicktype::ghost g : map_data.get_ghosts())
    {
        std::cout << "COLOR: " << g.get_color() << '\n';
        Color color = stringToColor(g.get_color());
        std::optional<Coordinate> scatterPointCoord = board.getScatterPoint(g.get_scatter());
        std::unique_ptr<MovementStrategy> strategy = strategyFromString(g.get_mode());
        auto optCoord = makeCoordinate(g.get_pos());

        if (!scatterPointCoord || strategy == nullptr || !optCoord)
        {
            continue;
        }

        ghosts.push_back(
            std::make_unique<Ghost>(color, optCoord.value(), scatterPointCoord.value(), std::move(strategy), player));
    }

    return ghosts;
}


static void moveCursorHome()
{
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
}

static void drawGrid(Player& player, std::vector<std::unique_ptr<Ghost>>& ghosts, Board const& board)
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
            if (!board.isEnabled(actualPos))
            {
                symbol = applyColor(Color::Blue, "██");
            }
            else if (board.hasPoint(actualPos))
            {
                symbol = applyColor(Color::White, ". ");
            }
            else
            {
                symbol = "  ";
            }

            for (auto& ghost : ghosts)
            {
                if (actualPos == ghost->getPosition())
                    symbol = applyColor(ghost->getColor(), "👻");
            }

            if (player.getPosition() == actualPos)
            {
                symbol = applyColor(Color::Yellow, player.getSymbol());
            }

            frame << symbol;
        }
        frame << "\n";
    }
    frame << "\n";

    std::cout << frame.str();
}

void tryChangeDirection(Player& player, Direction dirBuffer, Board const& board)
{
    Coordinate newPos = player.getPosition();
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
        player.setDirection(dirBuffer);
    }
}

//===========================
//          Main
//===========================
int main()
{
    //CREDIT TO: Franco for the emojis
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
    int counter = 0;
    constexpr auto kFrame = std::chrono::milliseconds(100);    

    srand(time(NULL));

    // GAME
    Board board = loadMap("../assets/map1.json");
    Player player(board.getPlayerStartingPoint(), Direction::None);
    std::vector<std::unique_ptr<Ghost>> ghosts = loadGhost("../assets/map1.json", board, &player);
    Direction dirBuffer = Direction::None;

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

        // PLAYER MOVEMENT
        player.update(board);

        // CHECK FOR POINT
        if (board.hasPoint(player.getPosition()))
        {
            board.setPoint(player.getPosition(), false);
        }

        // GHOSTS MOVEMENT
        for (auto& ghost : ghosts)
        {
            ghost->update(board);
        }


        // RENDER
        moveCursorHome();
        drawGrid(player, ghosts, board);

        // CHECK FOR WIN
        if (board.allPointsCollected())
        {
            std::cout << "¡Ganaste!\n";
            break;
        }

        // FRAME LIMITER
        auto nextFrame = now + kFrame;
        std::this_thread::sleep_until(nextFrame);
    }
}
