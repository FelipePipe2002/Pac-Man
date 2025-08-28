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
constexpr auto FRAME_DURATION = 150ms;
//===========================
//          Main
//===========================
int main()
{
    //CREDIT TO: Franco for the emojis
    //Init Console
    Console::initConsole();

    do
    {

        while (InputManager::enterPressed())
        {
            std::this_thread::sleep_for(10ms);
        }

        Renderer::showStartScreen();
        gameLogic::GameData gameData = gameLogic::initGame("../assets/map1.json");
        gameLogic::startGame(gameData);

        Direction dirBuffer = Direction::None;

        bool playing = true;
        unsigned int pointBefore = gameData.points;
        while (playing)
        {
            // --- INPUT ---
            if (InputManager::exitPressed())
                break;

            if (auto optDir = InputManager::readDirectionBuffer())
                dirBuffer = *optDir;

            tryChangeDirection(gameData.player, dirBuffer, gameData.board);

            // --- SAVE POINTS BEFORE THIS FRAME ---
            unsigned int pointsBefore = gameData.points;

            // --- UPDATE GAME STATE ---
            gameLogic::handlePlayerMovement(gameData, gameData.player);
            gameLogic::handleGhostsMovement(gameData.board, gameData.ghosts);
            gameLogic::handleGhostEaten(gameData);
            gameLogic::handleGhostBehavior(gameData);
            gameLogic::checkPelletFinish(gameData);

            unsigned int pointsDif = gameData.points - pointsBefore;
            //TODO: fix this shit
            // --- RENDER ---
            Console::moveCursorHome();
            Renderer::drawGrid(gameData.player, gameData.ghosts, gameData.board, gameData.points, pointsDif);

            // --- CHECK WIN/LOSE ---
            if (gameLogic::checkWinCondition(gameData.board))
            {
                std::cout << applyColor(Color::Green, "¡Ganaste!\n");
                playing = false;
            }
            else if (gameLogic::checkLoseCondition(gameData.ghosts, gameData.player))
            {
                std::cout << applyColor(Color::Red, "Perdiste!\n");
                playing = false;
            }

            // --- FRAME TIMING ---
            std::this_thread::sleep_for(FRAME_DURATION);
        }

    } while (Renderer::askPlayAgainScreen());
}
