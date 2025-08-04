
#pragma once
#include "CharacterClass.h"
#include "Card.h"

class SpellweaverClass : public CharacterClass {
public:
    SpellweaverClass();

    void OnTurnStart(Player* player) override;
    void UseClassAbility(Character* character, Player* player, std::shared_ptr<TriggerObserver> observer) override;

    // helper: store generated fragments per character
private:
    std::unordered_map<Character*, std::vector<std::shared_ptr<Card>>> pendingFragments;
};
