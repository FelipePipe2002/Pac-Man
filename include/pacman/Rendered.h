#pragma once
#include <string>
#include <memory>

#include "pacman/entities/Player.h"
#include "pacman/entities/Ghost.h"

namespace Renderer
{
    std::string centerText(std::string const& text, int consoleWidth);
    void drawGrid(Player& player,
        std::vector<std::unique_ptr<Ghost>>& ghosts,
        Board const& board,
        unsigned int points,
        unsigned int pointsDif);
    void showStartScreen();
    bool askPlayAgainScreen();

    static int displayWidth(std::string const& text);
    static std::string colorizeAndCenter(std::string const& text, int consoleWidth, int frameIndex = 0);

} // namespace Renderer