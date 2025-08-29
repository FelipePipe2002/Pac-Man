#pragma once

#include "pacman/Board.h"
#include "pacman/entities/Ghost.h"
#include "pacman/entities/Player.h"

#include <vector>

namespace gameLogic
{
    struct GameData
    {
        Board board;
        Player player;
        std::vector<std::unique_ptr<Ghost>> ghosts;

        GhostState state = GhostState::Scatter;
        std::chrono::time_point<std::chrono::system_clock> gameTime;

        unsigned int points = 0;
        unsigned int ghostPointMultiplier = 1;

        bool pelletEaten = false;
        std::chrono::time_point<std::chrono::system_clock> pelletTime;

        int pacmanCounter = 0;
        int ghostCounter = 0;
        int frightenedCounter = 0;
        int eatenCounter = 0;

        static constexpr int pacmanFrames = 6;     // 150 ms
        static constexpr int ghostFrames = 6;      // 150 ms Scatter/Chase
        static constexpr int frightenedFrames = 7; // 175 ms Frightened
        static constexpr int eatenFrames = 5;      // 125 ms Eaten

        bool fruitActive = false;
        std::chrono::time_point<std::chrono::system_clock> fruitSpawnTime;
        int fruitAppearanceCount = 0; // Number of times the fruit has appeared max 2

        GameData(Board&& b, Player&& p, std::vector<std::unique_ptr<Ghost>>&& g)
        : board(std::move(b)), player(std::move(p)), ghosts(std::move(g))
        {
        }
    };

    GameData initGame(std::string mapFile);
    void startGame(GameData& gameData);
    void updateFrameCounters(GameData& gameData);

    void handlePlayerMovement(GameData& gameData, Player& player);

    void handleGhostsMovement(GameData& gameData, std::vector<std::unique_ptr<Ghost>>& ghosts);
    void handleGhostBehavior(GameData& gameData);
    void handleGhostEaten(GameData& gameData);
    void endFrightenedMode(GameData& gameData);

    void handleFruit(GameData& gameData);

    bool checkWinCondition(Board const& board);
    bool checkLoseCondition(std::vector<std::unique_ptr<Ghost>> const& ghosts, Player const& player);

    static bool checkCollition(Entity const& e1, Entity const& e2);
    static Board loadMap(std::string const& filename);
    static std::vector<std::unique_ptr<Ghost>> loadGhost(
        std::string const& filename, Board const& board, Player* player);

} // namespace gameLogic