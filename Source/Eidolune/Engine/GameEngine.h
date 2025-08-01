
#pragma once
#include <memory>
#include "GameState.h"

class GameEngine {
public:
    GameEngine();
    void Run();
    void RunCombatLoop();
    void RunDeckEditor();
    void RunCharacterCustomization();
    void RunGacha();
    void RunSaveLoad();

private:
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
};
