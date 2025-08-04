
#pragma once
#include "CharacterClass.h"


class ShadeClass : public CharacterClass {
public:
    ShadeClass();
    void OnTurnStart(Player* player) override;
};