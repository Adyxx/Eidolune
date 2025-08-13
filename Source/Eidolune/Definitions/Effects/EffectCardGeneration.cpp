
/*
For creating or transforming cards mid-game, no matter where they come from.
Covers:
Summoning tokens or units
Discover/Generate-from-outside effects
Merging/fusing cards into new ones
Upgrading/evolving
Copying cards from other zones
*/

#include "Target.h"
#include <iostream>
#include "CardUtils.h"
#include "EffectContext.h"
#include "EffectRegistry.h"

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

void RegisterEffectFunctions_CardGeneration() {
    std::cout << "📦 Registering CardGeneration effects...\n";

    EffectRegistry::Instance().Register("summon", Summon);

}
