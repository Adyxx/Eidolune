

#pragma once
#include "CharacterClass.h"


class MachinistClass : public CharacterClass {
public:
    MachinistClass();
    void OnTurnStart(Player* player) override;
};