
#pragma once
#include "CharacterClass.h"

class SpecialistClass : public CharacterClass {
public:
    SpecialistClass();
    void OnTurnStart(GameState* state, Player* player) override;
};