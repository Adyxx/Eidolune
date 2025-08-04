#pragma once
#include "CharacterClass.h"

class BloodboundClass : public CharacterClass {
public:
    BloodboundClass();
    void OnTurnStart(Player* player) override;
    void ApplySetupBonus(Character* character) override;

    int GetBloodEcho(Character* character);
    void TrackHealthChange(Character* character, int deltaHP);
private:
    std::unordered_map<Character*, int> bloodEcho;
};
