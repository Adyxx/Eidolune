
#pragma once
#include <string>
#include <memory>

class GameState;
class Player;

class CharacterClass {
public:
    std::string Name;
    std::string Description;

    virtual void OnTurnStart(GameState* state, Player* player) {}
    virtual void OnCardPlayed(GameState* state, Player* player) {}
    virtual void ApplySetupBonus(Player* player) {}

    virtual ~CharacterClass() = default;
};
