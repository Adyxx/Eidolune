#include "BloodboundClass.h"
#include "../Abilities/ClassAbilities/BloodboundAbilities.h"
#include <iostream>

BloodboundClass::BloodboundClass() {
    std::cout << "hello from BloodboundClass::BloodboundClass()\n";
    Name = "Bloodbound";
    Description = "Harness life and death to fuel your power.";
    InitializeTree();
}

void BloodboundClass::ApplySetupBonus(Character* /*character*/) {
    // All setup is now done in Player constructor based on DeckRef->MainCharacter

}

void BloodboundClass::OnTurnStart(Player* player) {
    if (!player || !player->BloodboundData) return;

    std::cout << "[Bloodbound Turn Start] Echo: "
              << player->BloodboundData->BloodEcho << "\n";

    CheckAndUnlockNodes(player);
}

void BloodboundClass::CheckAndUnlockNodes(Player* player) {
    auto& bb = *player->BloodboundData;

    for (auto& [id, node] : NodeMap) {
        if (bb.BloodEcho >= node.EchoRequirement &&
            !bb.UnlockedNodes.count(id)) 
        {
            std::cout << "🔓 Bloodbound Node Unlocked: " << node.Name << "\n";
            node.ApplyEffect(player); // effect lambda still gets Player*
            bb.UnlockedNodes.insert(id);
        }
    }
}

void BloodboundClass::InitializeTree() {
    RegisterBloodboundAbilities(NodeMap);
}
