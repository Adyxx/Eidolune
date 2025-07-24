
#pragma once
#include <memory>
#include "GameState.h"

class GameEngine {
public:
    GameEngine(std::shared_ptr<Player> p1, std::shared_ptr<Player> p2);
    void Run();

private:
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
};
