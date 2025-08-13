
#pragma once
#include "CharacterClass.h"
#include "Card.h"

class SpellweaverClass : public CharacterClass {
public:
    SpellweaverClass();

    void OnTurnStart(Player* player) override;
    void TryMergeFragments(Player* player);
   
private:

};
