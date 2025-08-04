
#pragma once
#include "CharacterClass.h"



class GamblerClass : public CharacterClass {
public:
    GamblerClass();
    void OnTurnStart(Player* player) override;
};