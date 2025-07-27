
#pragma once
#include "CharacterClass.h"

class OathboundClass : public CharacterClass {
public:
    OathboundClass();
    void OnTurnStart(GameState* state, Player* player) override;
};
