#include "pacman/GameLogic.h"

#include <fstream>

namespace gameLogic
{

    GameData initGame(std::string mapFile)
    {
        Board board = loadMap("../assets/map1.json");
        GameData gameData{std::move(board), Player({0, 0}, Direction::None), {}};

        gameData.player = Player(gameData.board.getPlayerStartingPoint(), Direction::None);

        gameData.ghosts = loadGhost("../assets/map1.json", gameData.board, &gameData.player);

        return gameData;
    }

    void startGame(GameData& gameData)
    {
        for (auto& ghost : gameData.ghosts)
        {
            ghost->setState(GhostState::Scatter);
        }
        gameData.gameTime = std::chrono::system_clock::now();
    }

    void handlePlayerMovement(GameData& gameData, Player& player)
    {
        // PLAYER MOVEMENT
        player.update(gameData.board);

        // CHECK FOR POINT
        if (gameData.board.hasPoint(player.getPosition()))
        {
            gameData.board.setPoint(player.getPosition(), false);
            gameData.points += 10;
        }

        // CHECK IF OVER PELLET
        if (gameData.board.hasAPellet(player.getPosition()))
        {
            gameData.board.setPellet(player.getPosition(), false);
            gameData.pelletEaten = true;
            gameData.points += 50;
            gameData.pelletTime = std::chrono::system_clock::now();
            for (auto& ghost : gameData.ghosts)
            {
                ghost->setState(GhostState::Frightened);
            }
        }
    }

    void handleGhostsMovement(Board& board, std::vector<std::unique_ptr<Ghost>>& ghosts)
    {
        for (auto& ghost : ghosts)
        {
            ghost->update(board);
        }
    }

    void handleGhostBehavior(GameData& gameData)
    {
        if (!gameData.pelletEaten)
        {
            auto now = std::chrono::system_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - gameData.gameTime).count();
            GhostState aux = gameData.state;

            if (gameData.state == GhostState::Scatter && elapsed >= 8) //8s of dscatter
            {
                gameData.state = GhostState::Chase;
                gameData.gameTime = std::chrono::system_clock::now();
            }
            else if (gameData.state == GhostState::Chase && elapsed >= 20) //20s of chase
            {
                gameData.state = GhostState::Scatter;
                gameData.gameTime = std::chrono::system_clock::now();
            }
            if (aux != gameData.state)
            {
                for (auto& ghost : gameData.ghosts)
                {
                    ghost->setState(gameData.state);
                }
            }
        } 

        for (auto& ghost : gameData.ghosts)
        {
            if (ghost->getState() == GhostState::Eaten && ghost->getPosition() == gameData.board.getGhostHome())
            {
                ghost->setState(GhostState::Scatter);
                ghost->setColor(ghost->getOriginalColor());
            }
        }
    }

    void checkPelletFinish(GameData& gameData)
    {
        if (!gameData.pelletEaten)
            return;

        auto now = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - gameData.pelletTime).count();

        if (8 <= elapsed)
        {
            for (auto& ghost : gameData.ghosts)
            {
                ghost->setColor(ghost->getOriginalColor());
                ghost->setState(GhostState::Scatter);
            }
            gameData.pelletEaten = false;
            gameData.ghostPointMultiplier = 1;
        }
    }

    void handleGhostEaten(GameData &gameData)
    {
        if (gameData.pelletEaten)
        {
            for (auto& ghost : gameData.ghosts)
            {
                if (checkCollition(*ghost, gameData.player) && ghost->getState() == GhostState::Frightened)
                {
                    ghost->setState(GhostState::Eaten);
                    gameData.points += 200 * gameData.ghostPointMultiplier;
                    gameData.ghostPointMultiplier *= 2;
                }
            }
        }
    }

    bool checkWinCondition(Board const& board)
    {
        if (board.allPointsCollected())
        {
            return true;
        }
        return false;
    }

    bool checkLoseCondition(std::vector<std::unique_ptr<Ghost>> const& ghosts, Player const& player)
    {
        for (auto const& g : ghosts)
        {
            if (checkCollition(*g, player) && (g->getState() != GhostState::Frightened)
                && (g->getState() != GhostState::Eaten))
            {
                return true;
            }
        }
        return false;
    }

    static bool checkCollition(Entity const &e1, Entity const &e2)
    {
        if ((e1.getPosition() == e2.getPosition()))
        {
            return true;
        }
        if ((e2.getPosition() == e1.getLastPosition()) && (e2.getLastPosition() == e1.getPosition()))
        {
            return true;
        }
        return false;
    }

    static Board loadMap(std::string const& filename)
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

    static std::vector<std::unique_ptr<Ghost>> loadGhost(
        std::string const& filename, Board const& board, Player* player)
    {
        std::ifstream map_file(filename);
        if (!map_file.is_open())
        {
            throw std::runtime_error("No se pudo abrir el archivo de mapa");
        }

        nlohmann::json j;
        map_file >> j;
        quicktype::mapJson map_data = j.get<quicktype::mapJson>();

        std::vector<std::unique_ptr<Ghost>> ghosts;

        for (quicktype::ghost g : map_data.get_ghosts())
        {
            Color color = stringToColor(g.get_color());
            std::optional<Coordinate> scatterPointCoord = board.getScatterPoint(g.get_scatter());
            std::unique_ptr<MovementStrategy> strategy = strategyFromString(g.get_mode());
            auto optCoord = makeCoordinate(g.get_pos());

            if (!scatterPointCoord || strategy == nullptr || !optCoord)
            {
                continue;
            }

            ghosts.push_back(std::make_unique<Ghost>(
                color, optCoord.value(), scatterPointCoord.value(), std::move(strategy), player));
        }
        return ghosts;
    }
} // namespace gameLogic