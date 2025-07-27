
#pragma once
#include "CharacterClass.h"



class GamblerClass : public CharacterClass {
public:
    GamblerClass();
    void OnTurnStart(GameState* state, Player* player) override;
};