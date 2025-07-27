

#pragma once
#include "CharacterClass.h"


class MachinistClass : public CharacterClass {
public:
    MachinistClass();
    void OnTurnStart(GameState* state, Player* player) override;
};