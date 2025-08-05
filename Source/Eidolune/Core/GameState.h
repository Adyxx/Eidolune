#pragma once

#include <vector>
#include <memory>

#include "TriggerObserver.h"

#include "Player.h"

class GameState {
public:
    std::vector<std::shared_ptr<Player>> Players;
    int CurrentPlayer = 0;
    bool GameOver = false;

    std::shared_ptr<TriggerObserver> Observer = std::make_shared<TriggerObserver>();

    std::shared_ptr<Player> GetCurrentPlayer();
    std::shared_ptr<Player> GetOpponent();
    void NextTurn();

    std::shared_ptr<TriggerObserver> GetTriggerObserver() {
        return Observer;
    }
};

