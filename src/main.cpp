#include "pacman/Board.h"
#include "pacman/Entities/Ghost.h"
#include "pacman/Entities/Player.h"
#include "pacman/GameLogic.h"
#include "pacman/Rendered.h"
#include "pacman/utils/Color.h"
#include "pacman/utils/Console.h"
#include "pacman/utils/Coordinate.h"
#include "pacman/utils/Direction.h"
#include "pacman/utils/InputManager.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>


//===========================
//          Utils
//===========================

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

using namespace std::chrono_literals;
constexpr auto FRAME_DURATION = 25ms;
//===========================
//          Main
//===========================
int main()
{
    //CREDIT TO: Franco for the emojis
    //Init Console
    Console::initConsole();

    std::vector<std::string> displayNames;
    std::vector<std::string> fileNames;

    for (auto& entry : std::filesystem::directory_iterator("../maps/"))
    {
        if (entry.path().extension() == ".json")
        {
            std::string fname = entry.path().filename().string();
            fileNames.push_back(fname);

            std::string display = entry.path().stem().string();
            std::replace(display.begin(), display.end(), '_', ' ');
            displayNames.push_back(display);
        }
    }

    bool win = false;

    do
    {
        while (InputManager::enterPressed()) //tiny buffer so it doesn't auto skip the start screen
        {
            std::this_thread::sleep_for(10ms);
        }

        int selectedIndex = Renderer::showStartScreen(displayNames);

        std::string selectedFile = fileNames[selectedIndex];

        gameLogic::GameData gameData = gameLogic::initGame("../maps/" + selectedFile);
        gameLogic::startGame(gameData);

        Direction dirBuffer = Direction::None;
        bool playing = true;

        while (playing)
        {
            // --- INPUT ---
            if (InputManager::exitPressed())
                break;

            if (auto optDir = InputManager::readDirectionBuffer())
                dirBuffer = *optDir;

            tryChangeDirection(gameData.player, dirBuffer, gameData.board);


            // --- UPDATE ENTITY MOVEMENT ---
            gameLogic::updateFrameCounters(gameData);


            // --- SAVE POINTS BEFORE THIS FRAME ---
            unsigned int pointsBefore = gameData.points;


            // --- UPDATE GAME STATE ---
            gameLogic::handlePlayerMovement(gameData, gameData.player);
            gameLogic::handleGhostsMovement(gameData, gameData.ghosts);
            gameLogic::handleGhostEaten(gameData);
            gameLogic::handleGhostBehavior(gameData);
            gameLogic::endFrightenedMode(gameData);

            // --- HANDLE FRUIT ---
            gameLogic::handleFruit(gameData);

            // --- CALCULATE POINTS DIFFERENCE ---
            unsigned int pointsDif = gameData.points - pointsBefore;

            // --- RENDER ---
            Console::moveCursorHome();
            Renderer::drawGrid(gameData.player, gameData.ghosts, gameData.board, gameData.points, pointsDif);

            // --- CHECK WIN/LOSE ---
            if (gameLogic::checkWinCondition(gameData.board))
            {
                win = true;
                playing = false;
            }
            else if (gameLogic::checkLoseCondition(gameData.ghosts, gameData.player))
            {
                win = false;
                playing = false;
            }

            // --- FRAME TIMING ---
            std::this_thread::sleep_for(FRAME_DURATION);
        }

    } while (Renderer::askPlayAgainScreen(win));
}
