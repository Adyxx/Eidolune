
/*
For creating or transforming cards mid-game, no matter where they come from.
Covers:
Summoning tokens or units
Discover/Generate-from-outside effects
Merging/fusing cards into new ones
Upgrading/evolving
Copying cards from other zones

discover_card
copy_card
replace_card
*/

#include "Target.h"
#include <iostream>
#include "CardUtils.h"
#include "EffectContext.h"
#include "EffectRegistry.h"

#include "EffectHelpers.h"
#include "Types.h"
#include "CardUtils.h"

void Summon(void* source, Target target, std::optional<int> value) {
    auto binding = GetEffectCallContext().CurrentBinding;
    if (!binding) {
        std::cout << "❌ Summon: No binding in context\n";
        return;
    }

    auto* sourceCard = static_cast<GameCard*>(source);
    if (!sourceCard || !binding->LinkedCard) {
        std::cout << "❌ Invalid source or no linked card\n";
        return;
    }

    auto* player = static_cast<Player*>(target.ptr);
    if (!player || !player->Observer) return;

    auto summoned = CardUtils::RegisterCardMidGame(binding->LinkedCard, player, CardZone::BOARD, player->Observer);
    if (!summoned) return;

    std::cout << "⚔️ Summoned: " << summoned->GetName() << " for " << player->GetName() << "\n";
}


void MixInto(void* source, Target target, std::optional<int> value) {
    auto* targetCard = static_cast<GameCard*>(target.ptr);
    auto* sourceCard = static_cast<GameCard*>(source);

    if (!targetCard || !sourceCard) {
        std::cout << "[Effect] MixInto: Invalid source or target card\n";
        return;
    }

    auto* player = sourceCard->Owner; // assuming GameCard has Owner
    if (!player) {
        std::cout << "[Effect] MixInto: Source card has no owner\n";
        return;
    }

    // 1. Eligibility check
    if (!(targetCard->Model->AuxiliaryType == AuxiliaryCardType::REAGENT || targetCard->Model->AuxiliaryType == AuxiliaryCardType::CONSUMABLE)) {
        std::cout << "[Effect] MixInto: Target not eligible for mixing\n";
        return;
    }

    // 2. Add target's effects to source
    for (auto& binding : targetCard->RuntimeEffectBindings) {
        sourceCard->RuntimeEffectBindings.push_back(std::make_shared<CardEffectBinding>(*binding));
    }

    // 3. Exile target card
    EffectHelpers::MoveCardToZone(targetCard, player, player->Hand, player->ExileZone, "Hand", "Exile");

    std::cout << "[Effect] MixInto: Added " 
              << targetCard->RuntimeEffectBindings.size() 
              << " effects to " << sourceCard->GetName() << ".\n";
}





void RegisterEffectFunctions_CardGeneration() {
    std::cout << "📦 Registering CardGeneration effects...\n";

    EffectRegistry::Instance().Register("summon", Summon);

    /// NOT TESTED...
    EffectRegistry::Instance().Register("mix_into", MixInto);

}
