
#pragma once
#include "CharacterClass.h"

class SoulforgedClass : public CharacterClass {
public:
    SoulforgedClass();
    void OnTurnStart(Player* player) override;
};