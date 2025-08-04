
#include "EffectDefinitions.h"
#include "EffectRegistry.h"
#include "../Core/DeckCard.h"
#include "../Core/Player.h"
#include "../Core/GameCard.h"
#include <iostream>
#include "TriggerObserver.h"
#include "GameActions.h"

void DrawCard(void* source, Target target, std::optional<int> value) {
    if (target.type != Target::Type::PLAYER) {
        std::cout << "[Effect] DrawCard: Invalid player target\n";
        return;
    }

    auto* player = static_cast<Player*>(target.ptr);

    if (!player) {
        std::cout << "[Effect] DrawCard: Invalid player target\n";
        return;
    }

    int drawAmount = value.value_or(1);
    std::cout << "[Effect] Drawing " << drawAmount << " card(s) for " << player->GetName() << "\n";

    for (int i = 0; i < drawAmount; ++i) {
        if (player->DrawPile.empty()) {
            std::cout << "❌ Deck is empty. Cannot draw.\n";
            break;
        }

        std::cout << player->Hand.size() << "  /  " << player->MaxHandSize <<"\n";
        if (player->Hand.size() >= player->MaxHandSize) {
            std::cout << "⚠️ Hand full. Skipping draw.\n";
            break;
        }

        auto card = player->DrawPile.back();
        player->DrawPile.pop_back();
        card->Owner = player;
        card->Zone = CardZone::HAND;
        player->Hand.push_back(card);

        std::cout << "🃏 Drew card: " << card->GetName() << "\n";

        //trigger_observer->Emit("card_drawn", player, card);
    }
}

void ApplyHaste(void* source, Target  target, std::optional<int> value) {
    std::cout << "[Haste] triggered.\n";
}

void OverrideDeckLimit(void* source, Target  target, std::optional<int> value) {
    // this is on-deck-build effect and is implemented elsewhere.
}

void DealDamage(void* source, Target target, std::optional<int> value) {
    int damage = value.value_or(1);
    std::cout << "[Effect] DealDamage for " << damage << "\n";

    GameActions::ResolveDamage(source, target, damage, "effect");
}

void ExileSelf(void* source, Target target, std::optional<int>) {
    auto* card = static_cast<GameCard*>(source);
    if (!card || !card->Owner) return;

    std::cout << "☠️ Exiling " << card->GetName() << "\n";
    card->Zone = CardZone::EXILE;
}

void Aura_OverallStatBoost(void* source, Target, std::optional<int> value) {
    if (!source) return;

    auto* sourceCard = static_cast<GameCard*>(source);
    if (!sourceCard || !sourceCard->Owner) return;

    int buffAmount = value.value_or(1);
    auto& board = sourceCard->Owner->Board;

    for (auto& card : board) {
        if (!card) continue;

        if (card->ActiveAuras.count(sourceCard->Id)) {
            // Already applied
            continue;
        }

        // Apply the aura
        card->ApplyAura(sourceCard->Id, buffAmount, buffAmount);

        std::cout << "🟢 Aura from [" << sourceCard->GetName()
                  << "] applied to [" << card->GetName()
                  << "] (+" << buffAmount << "/+" << buffAmount << ")\n";
    }
}



void RegisterEffectFunctions() {
    std::cout << "📦 Registering core effect functions...\n";
    EffectRegistry::Instance().Register("draw_card", DrawCard);
    EffectRegistry::Instance().Register("apply_haste", ApplyHaste);
    EffectRegistry::Instance().Register("override_deck_limit", OverrideDeckLimit);
    EffectRegistry::Instance().Register("deal_damage", DealDamage);
    EffectRegistry::Instance().Register("Aura_OverallStatBoost", Aura_OverallStatBoost);
    std::cout << "✅ Core effect functions registered.\n";
}