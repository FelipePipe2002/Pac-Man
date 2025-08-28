#pragma once

#include "pacman/Board.h"
#include "pacman/Entities/Entity.h"
#include "pacman/Entities/MovementStrategy.h"
#include "pacman/utils/Color.h"
#include "pacman/utils/Coordinate.h"

enum class GhostState
{
    Chase,
    Scatter,
    Frightened,
    Eaten
};

struct Player;

class Ghost : public Entity
{
public:
    Ghost(Color color, Coordinate initPos, Coordinate scatterPoint, std::unique_ptr<MovementStrategy> chaseStrategy, Player* target);
    ~Ghost() = default;

    void move(Board const& board) override;

    [[nodiscard]] GhostState getState() const;
    [[nodiscard]] Color getColor() const;
    [[nodiscard]] Color getOriginalColor() const;
    [[nodiscard]] Coordinate getScatterPoint() const;

    void setState(GhostState newState);
    void setColor(Color color);
    void setTarget(Player& target);

private:
    Color mColor;
    Color mOriginalColor;
    GhostState mState = GhostState::Chase;
    Coordinate mScatterPoint;
    Player* mTarget;

    std::unique_ptr<MovementStrategy> mChaseStrategy;
    std::unique_ptr<MovementStrategy> mScatterStrategy;
    std::unique_ptr<MovementStrategy> mFrightenedStrategy;
    std::unique_ptr<MovementStrategy> mEatenStrategy;
};
