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
        std::chrono::time_point<std::chrono::system_clock> gameTime;
        GhostState state = GhostState::Scatter;
        unsigned int points = 0;
        unsigned int ghostPointMultiplier = 1;

        bool pelletEaten = false;
        std::chrono::time_point<std::chrono::system_clock> pelletTime;

        GameData(Board&& b, Player&& p, std::vector<std::unique_ptr<Ghost>>&& g)
        : board(std::move(b)), player(std::move(p)), ghosts(std::move(g))
        {
        }
    };

    GameData initGame(std::string mapFile);
    void startGame(GameData& gameData);

    void handlePlayerMovement(GameData& gameData, Player& player);
    void handleGhostsMovement(Board& board, std::vector<std::unique_ptr<Ghost>>& ghosts);

    void handleGhostBehavior(GameData& gameData);

    void checkPelletFinish(GameData& gameData);
    void handleGhostEaten(GameData& gameData);

    bool checkWinCondition(Board const& board);
    bool checkLoseCondition(std::vector<std::unique_ptr<Ghost>> const& ghosts, Player const& player);

    static bool checkCollition(Entity const& e1, Entity const& e2);
    static Board loadMap(std::string const& filename);
    static std::vector<std::unique_ptr<Ghost>> loadGhost(
        std::string const& filename, Board const& board, Player* player);

} // namespace gameLogic