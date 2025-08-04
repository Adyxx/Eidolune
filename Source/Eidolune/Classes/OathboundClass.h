
#pragma once
#include "CharacterClass.h"

class OathboundClass : public CharacterClass {
public:
    OathboundClass();
    void OnTurnStart(Player* player) override;
};
