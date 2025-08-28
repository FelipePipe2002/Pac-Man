#pragma once
#include <optional>
#include "Direction.h"

namespace InputManager
{
    std::optional<Direction> readDirectionBuffer();

    bool exitPressed();
    bool enterPressed();
};