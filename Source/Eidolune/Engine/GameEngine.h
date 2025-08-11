
#pragma once
#include <memory>
#include "GameState.h"
#include "UserData.h"

class GameEngine {
public:
    std::shared_ptr<User> PromptUserLogin();
    GameEngine();
    void Run();
    void RunCombatLoop();
    void RunDeckEditor();
    void RunCharacterCustomization();
    void RunGacha();

    std::shared_ptr<User> currentUser;
    UserData currentUserData;
    
private:
    std::shared_ptr<Player> player1;
    std::shared_ptr<Player> player2;
};
