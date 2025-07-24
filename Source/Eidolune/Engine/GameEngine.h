
#pragma once
#include <memory>
#include "GameState.h"

class GameEngine {
public:
    GameEngine();
    void Run();

private:
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
};
