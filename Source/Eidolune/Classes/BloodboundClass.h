#pragma once
#include "CharacterClass.h"
#include <unordered_map>
#include <functional>
#include <vector>

#include "Character.h"
#include "../Abilities/Other/AbilityHelpers/BloodboundNode.h"

class BloodboundClass : public CharacterClass {
public:
    BloodboundClass();

    void OnTurnStart(Player* player) override;
    void ApplySetupBonus(Character* character) override;

    void CheckAndUnlockNodes(Player* player);

private:
    std::unordered_map<int, BloodboundNode> NodeMap; // All defined nodes
    //int RootNodeId = 1;

    void InitializeTree();
};
