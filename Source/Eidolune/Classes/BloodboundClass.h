#pragma once
#include "CharacterClass.h"

class BloodboundClass : public CharacterClass {
public:
    BloodboundClass();
    void OnTurnStart(GameState* state, Player* player) override;
};