#include "pacman/Rendered.h"
#include "pacman/utils/Console.h"
#include "pacman/utils/InputManager.h"
#include <iostream>
#include <thread>

namespace Renderer
{
    std::string centerText(std::string const& text, int consoleWidth)
    {
        int pad = (consoleWidth - static_cast<int>(text.size())) / 2;
        if (pad < 0)
            pad = 0;
        return std::string(pad, ' ') + text;
    }

    void drawGrid(Player& player,
        std::vector<std::unique_ptr<Ghost>>& ghosts,
        Board const& board,
        unsigned int points,
        unsigned int pointsDif)
    {
        std::ostringstream frame;
        frame << "ESC: quit | WASD: move\n";

        frame << "\033[2K\r"; // borra la línea actual y mueve el cursor al inicio
        if (pointsDif > 0)
        {
            std::string msg = " + " + std::to_string(pointsDif);
            frame << "Points: " << points << applyColor(Color::LightYellow, msg);
        }
        else
        {
            frame << "Points: " << points;
        }
        frame << "\n";


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
                    symbol = applyColor(board.getMapColor(), "██");
                }
                else if (board.hasFruit(actualPos))
                {
                    symbol = applyColor(Color::Red, "🍒");
                }
                else if (board.hasPoint(actualPos) && !board.hasAPellet(actualPos))
                {
                    symbol = applyColor(Color::White, ". ");
                }
                else if (board.hasAPellet(actualPos))
                {
                    symbol = applyColor(Color::White, "⚈ ");
                }
                else
                {
                    symbol = "  ";
                }

                for (auto& ghost : ghosts)
                {
                    if (actualPos == ghost->getPosition())
                    {
                        if (ghost->getState() == GhostState::Eaten)
                        {
                            symbol = applyColor(Color::LightBlue, "👀");
                        }
                        else
                        {
                            symbol = applyColor(ghost->getColor(), "👻");
                        }
                    }
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

    int showStartScreen(std::vector<std::string> const& maps)
    {
        std::string animation[] = {
            "👻☕☕☕☕☕☕",
            "👻👻☕☕☕☕☕",
            "👻👻👻☕☕☕☕",
            "👻👻👻👻☕☕☕",
            "☕👻👻👻👻☕☕",
            "☕☕👻👻👻👻☕",
            "🌜☕☕👻👻👻👻",
            "☕🌜☕☕👻👻👻",
            "☕☕🌜☕☕👻👻",
            "☕☕☕🌜☕☕👻",
            "☕☕☕☕🌜☕☕",
            "☕☕☕☕☕🌜☕",
            "☕☕☕☕☕☕🌜",
            "☕☕☕☕☕☕☕",
            "☕☕☕☕☕☕☕",
            "☕☕☕☕☕☕🌛",
            "☕☕☕☕☕🌛☕",
            "☕☕☕☕🌛☕☕",
            "☕☕☕🌛☕☕👻",
            "☕☕🌛☕☕👻👻",
            "☕🌛☕☕👻👻👻",
            "🌛☕☕👻👻👻👻",
            "☕☕👻👻👻👻☕",
            "☕👻👻👻👻☕☕",
            "👻👻👻👻☕☕☕",
            "👻👻👻☕☕☕☕",
            "👻👻☕☕☕☕☕",
            "👻☕☕☕☕☕☕",
            "☕☕☕☕☕☕☕",
            "☕☕☕☕☕☕☕",
        };

        int totalFrames = sizeof(animation) / sizeof(animation[0]);
        int i = 0;
        int const consoleWidth = 80;
        size_t selectedMap = 0;

        std::string banner =
            R"(  
          ███████╗  █████╗  ██████╗     ███╗   ███╗ █████╗ ███╗   ██╗
          ██╔═══██╗██╔══██╗██╔════╝     ████╗ ████║██╔══██╗████╗  ██║
          ███████╔╝███████║██║          ██╔████╔██║███████║██╔██╗ ██║
          ██╔════╝ ██╔══██║██║          ██║╚██╔╝██║██╔══██║██║╚██╗██║
          ██║      ██║  ██║╚██████╗     ██║ ╚═╝ ██║██║  ██║██║ ╚████║
          ╚═╝      ╚═╝  ╚═╝ ╚═════╝     ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝
        )";

        while (true)
        {
            Console::moveCursorHome();


            std::istringstream iss(banner);
            std::string line;
            while (std::getline(iss, line))
            {
                std::cout << applyColor(Color::Yellow, centerText(line, consoleWidth)) << "\n";
            }

            std::cout << "\n";
            std::cout << colorizeAndCenter(animation[i], consoleWidth, i) << "\n";


            i = (i + 1) % totalFrames;

            std::cout << "\n" << applyColor(Color::Cyan, centerText("Select a map:", consoleWidth)) << "\n\n";
            for (size_t idx = 0; idx < maps.size(); ++idx)
            {
                std::string prefix = (idx == selectedMap) ? "> " : "  ";
                Color col = (idx == selectedMap) ? Color::Green : Color::White;
                std::cout << applyColor(col, centerText(prefix + maps[idx], consoleWidth)) << "\n";
            }

            if (auto dir = InputManager::readDirectionBuffer())
            {
                if (*dir == Direction::Up)
                {
                    selectedMap = (selectedMap == 0) ? maps.size() - 1 : selectedMap - 1;
                }
                else if (*dir == Direction::Down)
                {
                    selectedMap = (selectedMap + 1) % maps.size();
                }
            }

            std::cout << "\n"
                      << applyColor(Color::Green, centerText("Press ENTER to start...", consoleWidth)) << "\n";

            if (InputManager::enterPressed()) // ENTER
            {
                system("cls");
                return selectedMap;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    bool askPlayAgainScreen(bool win)
    {
        system("cls");
        int const consoleWidth = 80;

        std::string const winMessage = "You won! 🎉 ";
        std::string const loseMessage = "You lost! 💀 ";
        std::string const prompt = "Do you want to play again? ";
        std::string const instruction = "Press ENTER for yes, any ESC to exit";
        std::vector<std::vector<std::string>> const animation = {
            {"           ██████████           ", 
             "      ████████████████████      ", 
             "    ████████████████████████    ",
             "  ███████   ██████████████████  ", 
             " ██████████████████████████████ ",
             " ██████████████████████████████ ", 
             "████████████████████████████████",
             "████████████████████████████████", 
             "████████████████████████████████",
             " ██████████████████████████████ ", 
             " ██████████████████████████████ ",
             "  ████████████████████████████  ", 
             "    ████████████████████████    ",
             "      ████████████████████      ", 
             "           ██████████           "},
            {"           ██████████           ",
             "      ████████████████████      ",
             "    ████████████████████████    ",
             "  ███████   ██████████████████  ",
             " ██████████████████████████████ ",
             " █████████████████████████████  ",
             "███████████████████████         ",
             "██████████████                  ",
             "███████████████████████         ",
             " █████████████████████████████  ",
             " ██████████████████████████████ ",
             "  ████████████████████████████  ",
             "    ████████████████████████    ",
             "      ████████████████████      ",
             "           ██████████           "},
            {"           ██████████           ",
             "      ████████████████████      ",
             "    ████████████████████████    ",
             "  ███████   █████████████████   ",
             " █████████████████████████      ",
             " ██████████████████████         ",
             "█████████████████               ",
             "██████████████                  ",
             "█████████████████               ",
             " ██████████████████████         ",
             " █████████████████████████      ",
             "  ███████████████████████████   ",
             "    ████████████████████████    ",
             "      ████████████████████      ",
             "           ██████████           "},
            {"           ██████████           ",
             "      ████████████████████      ",
             "    ████████████████████████    ",
             "  ███████   ████████████        ",
             " ████████████████████           ",
             " ██████████████████             ",
             "████████████████                ",
             "██████████████                  ",
             "████████████████                ",
             " █████████████████              ",
             " ████████████████████           ",
             "  ██████████████████████        ",
             "    ███████████████████████     ",
             "      ████████████████████      ",
             "           ██████████           "}};

        size_t frame = 0;
        int direction = 1;
        while (true)
        {
            Console::moveCursorHome();
            std::cout << "\n\n";
            if (win)
            {
                std::cout << applyColor(Color::Green, centerText(winMessage, consoleWidth)) << "\n\n";
            }
            else
            {
                std::cout << applyColor(Color::Red, centerText(loseMessage, consoleWidth)) << "\n\n";
            }
            std::cout << applyColor(Color::Cyan, centerText(prompt, consoleWidth)) << "\n\n";
            std::cout << applyColor(Color::Yellow, centerText(instruction, consoleWidth)) << "\n\n";

            for (std::string const& line : animation[frame])
            {
                std::cout << applyColor(Color::Yellow, std::string(24, ' ') + line) << "\n";
            }
            frame += direction;

            if (frame == animation.size() - 1 || frame == 0)
            {
                direction *= -1;
            }

            if (InputManager::enterPressed())
            {
                system("cls");
                return true;
            }

            if (InputManager::exitPressed())
            {
                system("cls");
                return false;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        system("cls");
    }


    static int displayWidth(std::string const& text)
    {
        int width = 0;
        for (size_t i = 0; i < text.size();)
        {
            unsigned char c = text[i];
            if ((c & 0xF0) == 0xF0)
            {
                width += 2;
                i += 4;
            }
            else if ((c & 0xE0) == 0xE0)
            {
                width += 2;
                i += 3;
            }
            else if ((c & 0xC0) == 0xC0)
            {
                width += 1;
                i += 2;
            }
            else
            {
                width += 1;
                i += 1;
            }
        }
        return width;
    }

    static std::string colorizeAndCenter(std::string const& text, int consoleWidth, int frameIndex)
    {
        std::vector<Color> ghostColors = {Color::Red, Color::Magenta, Color::Cyan, Color::Green};
        int ghostCount = 0;
        std::string result;

        for (size_t i = 0; i < text.size();)
        {
            std::string emoji;
            size_t length = 1;

            unsigned char c = text[i];
            if ((c & 0xF0) == 0xF0 && i + 3 < text.size())
            {
                emoji = text.substr(i, 4);
                length = 4;
            }
            else if ((c & 0xF0) == 0xE0 && i + 2 < text.size())
            {
                emoji = text.substr(i, 3);
                length = 3;
            }
            else
            {
                emoji = text.substr(i, 1);
            }


            if (emoji == "🌜" || emoji == "🌛")
            {
                result += applyColor(Color::Yellow, emoji);
            }
            else if (emoji == "👻")
            {
                Color col =
                    (frameIndex > 10 && ghostCount < ghostColors.size()) ? ghostColors[ghostCount] : Color::DarkBlue;
                result += applyColor(col, emoji);
                ghostCount++;
            }
            else
            {
                result += applyColor(Color::Black, emoji);
            }

            i += length;
        }

        int pad = std::max(0, (consoleWidth - displayWidth(text)) / 2);
        return std::string(pad, ' ') + result;
    }
} // namespace Renderer