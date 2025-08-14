#include "BloodboundAbilities.h"
#include <iostream>

void RegisterBloodboundAbilities(std::unordered_map<int, BloodboundNode>& outNodes) {
    outNodes[1] = {
        1, "Initiate of the Red Path", "Gain +1 HP regen per turn.",
        10,
        [](Player* p) { if (p && p->BloodboundData) p->BloodboundData->RegenPerTurn += 1; },
        {2, 3}
    };

    outNodes[2] = {
        2, "Crimson Leech", "Leech effects cost -1.",
        20,
        [](Player* p) { if (p && p->BloodboundData) p->BloodboundData->CostReduction["Leech"] -= 1; },
        {4}
    };

    outNodes[3] = {
        3, "Sanguine Fury", "Deal +1 damage if you gained HP last turn.",
        20,
        [](Player* p) { if (p && p->BloodboundData) p->BloodboundData->HasSanguineFury = true; },
        {4}
    };

    outNodes[4] = {
        4, "Blood Sigil Mastery", "Activate sigils when you oscillate HP.",
        30,
        [](Player* p) { if (p && p->BloodboundData) p->BloodboundData->CanActivateBloodSigils = true; },
        {}
    };
}
