
#pragma once
#include "CharacterClass.h"

class SpellweaverClass : public CharacterClass {
public:
    SpellweaverClass();
    void OnTurnStart(GameState* state, Player* player) override;
};