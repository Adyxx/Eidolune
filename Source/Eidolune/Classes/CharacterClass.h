
#pragma once
#include <string>
#include <memory>
#include "../Core/Player.h"

class GameState;


class CharacterClass {
public:
    std::string Name;
    std::string Description;

    virtual void OnTurnStart(GameState* state, Player* player) {}
    virtual void OnCardPlayed(GameState* state, Player* player) {}
    virtual void ApplySetupBonus(Player* player) {}

    virtual ~CharacterClass() = default;
};
