
#pragma once
#include <string>
#include <memory>
#include "../Core/Player.h"
#include "TriggerObserver.h"
#include <iostream>

class Character;

class CharacterClass {
public:
    std::string Name;
    std::string Description;

    virtual void OnTurnStart(Player* player) {}
    virtual void OnCardPlayed(Player* player) {}
    virtual void ApplySetupBonus(Character* character) {}

    virtual void UseClassAbility(Character* character, Player* player, std::shared_ptr<TriggerObserver> observer) {
        std::cout << "⚠️ This class has no active ability.\n";
    }

    virtual ~CharacterClass() = default;
};
