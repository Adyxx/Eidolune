
#include "EffectDefinitions.h"
#include "EffectRegistry.h"
#include "../Core/DeckCard.h"
#include "../Core/Player.h"
#include "../Core/GameCard.h"
#include <iostream>
#include "TriggerObserver.h"
#include "GameActions.h"
/*
void DrawCard(void* source, void* target, std::optional<int> value) {

    auto* player = static_cast<Player*>(target);

    if (!player) {
        std::cout << "[Effect] DrawCard: Invalid player target\n";
        return;
    }

    int drawAmount = value.value_or(1);
    std::cout << "[Effect] Drawing " << drawAmount << " card(s) for " << player->GetName() << "\n";

    for (int i = 0; i < drawAmount; ++i) {
        if (player->DeckRef->DeckCards.empty()) {
            std::cout << "❌ Deck is empty. Cannot draw.\n";
            break;
        }

        std::cout << player->Hand.size() << "  /  " << player->MaxHandSize <<"\n";
        if (player->Hand.size() >= player->MaxHandSize) {
            std::cout << "⚠️ Hand full. Skipping draw.\n";
            break;
        }

        auto deckCard = player->DeckRef->DeckCards.back();
        player->DeckRef->DeckCards.pop_back();

        //auto card = std::make_shared<GameCard>(deckCard->CardRef);
        if (deckCard->GameCardCopies.empty()) {
            std::cout << "❌ No copies left for card: " << deckCard->CardRef->Name << "\n";
            continue;
        }
        auto card = deckCard->GameCardCopies.back();
        deckCard->GameCardCopies.pop_back();

        card->Owner = player;
        card->Zone = CardZone::HAND;
        player->Hand.push_back(card);

        std::cout << "🃏 Drew card: " << card->GetName() << "\n";

        //trigger_observer->Emit("card_drawn", player, card);
    }
}

void ApplyHaste(void* source, void* target, std::optional<int> value) {
    std::cout << "[Effect] ApplyHaste triggered\n";
}

void OverrideDeckLimit(void* source, void* target, std::optional<int> value) {
    std::cout << "[Effect] OverrideDeckLimit to " << (value.value_or(0)) << "\n";
}

void DealDamage(void* source, void* target, std::optional<int> value) {
    int damage = value.value_or(1);
    std::cout << "[Effect] DealDamage for " << damage << "\n";

    GameActions::ResolveDamage(source, target, damage, "effect");
}

*/


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
        if (player->DeckRef->DeckCards.empty()) {
            std::cout << "❌ Deck is empty. Cannot draw.\n";
            break;
        }

        std::cout << player->Hand.size() << "  /  " << player->MaxHandSize <<"\n";
        if (player->Hand.size() >= player->MaxHandSize) {
            std::cout << "⚠️ Hand full. Skipping draw.\n";
            break;
        }

        auto deckCard = player->DeckRef->DeckCards.back();
        player->DeckRef->DeckCards.pop_back();

        //auto card = std::make_shared<GameCard>(deckCard->CardRef);
        if (deckCard->GameCardCopies.empty()) {
            std::cout << "❌ No copies left for card: " << deckCard->CardRef->Name << "\n";
            continue;
        }
        auto card = deckCard->GameCardCopies.back();
        deckCard->GameCardCopies.pop_back();

        card->Owner = player;
        card->Zone = CardZone::HAND;
        player->Hand.push_back(card);

        std::cout << "🃏 Drew card: " << card->GetName() << "\n";

        //trigger_observer->Emit("card_drawn", player, card);
    }
}

void ApplyHaste(void* source, Target  target, std::optional<int> value) {

}

void OverrideDeckLimit(void* source, Target  target, std::optional<int> value) {

}

void DealDamage(void* source, Target target, std::optional<int> value) {
    int damage = value.value_or(1);
    std::cout << "[Effect] DealDamage for " << damage << "\n";

    GameActions::ResolveDamage(source, target, damage, "effect");
}



void InitEffectRegistry() {
    EffectRegistry::Register("apply_haste", { ApplyHaste, TargetHint::CARD });
    EffectRegistry::Register("draw_card", { DrawCard, TargetHint::PLAYER });
    EffectRegistry::Register("override_deck_limit", { OverrideDeckLimit, TargetHint::CARD });
    EffectRegistry::Register("deal_damage", { DealDamage, TargetHint::NONE });
}
