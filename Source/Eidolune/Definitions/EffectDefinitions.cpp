#include <iostream>

#include "EffectDefinitions.h"
#include "EffectRegistry.h"
#include "Player.h"
#include "GameCard.h"
#include "TriggerObserver.h"
#include "GameActions.h"
#include "CardUtils.h"
#include "EffectContext.h"

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
    auto* card = static_cast<GameCard*>(source);
    card->SummoningSickness = false;
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

    auto& player = card->Owner;

    // Try to find and move the card from any zone it's in
    auto tryMoveCard = [&](std::vector<std::shared_ptr<GameCard>>& zone, const std::string& zoneName) -> bool {
        auto it = std::find_if(zone.begin(), zone.end(),
            [&](const std::shared_ptr<GameCard>& c) {
                return c.get() == card;
            });

        if (it != zone.end()) {
            std::cout << "📤 Moving " << card->GetName() << " from " << zoneName << " to Exile.\n";
            player->ExileZone.push_back(std::move(*it));
            zone.erase(it);
            return true;
        }
        return false;
    };

    // Search zones
    bool moved = tryMoveCard(player->Hand, "Hand")
              || tryMoveCard(player->Board, "Board")
              || tryMoveCard(player->Graveyard, "Graveyard")
              || tryMoveCard(player->DrawPile, "DrawPile")
              || tryMoveCard(player->OathZone, "OathZone");

    if (!moved) {
        std::cout << "⚠️ Could not find card in any zone. Possible logic error.\n";
    }

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

        card->ApplyAura(sourceCard->Id, buffAmount, buffAmount);

        std::cout << "🟢 Aura from [" << sourceCard->GetName() << "] applied to [" << card->GetName() << "] (+" << buffAmount << "/+" << buffAmount << ")\n";
    }
}


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

void GetTimeCounter(void* source, Target, std::optional<int> value) {
    auto* card = static_cast<GameCard*>(source);
    if (card && value) {
        card->TimeCounter += value.value();
        std::cout << "⏳ Gained " << value.value() << " time counters.\n";
    }
}

void RemoveTimeCounter(void* source, Target, std::optional<int> value) {
    auto* card = static_cast<GameCard*>(source);
    if (card && value) {
        card->TimeCounter -= value.value();
        std::cout << "⌛ Removed " << value.value() << " time counters. Now: " << card->TimeCounter << "\n";
    }
}

void ChooseBasedOnTimeCounter(void* source, Target target, std::optional<int>) {
    auto* card = static_cast<GameCard*>(source);
    if (!card || !card->Model) return;

    int time = card->TimeCounter;
    std::string prefix = "time_" + std::to_string(time) + "_opt";

    std::vector<std::shared_ptr<CardEffectBinding>> matchingBindings;

    for (const auto& binding : card->Model->EffectBindings) {
        if (!binding || !binding->GetTrigger()) continue;

        const std::string& triggerName = binding->GetTrigger()->GetName();
        if (triggerName.rfind(prefix, 0) == 0) { // starts with prefix
            matchingBindings.push_back(binding);
        }
    }

    if (matchingBindings.empty()) {
        std::cout << "❌ No choice options for time_" << time << "\n";
        return;
    }

    std::shared_ptr<CardEffectBinding> chosenBinding;

    if (matchingBindings.size() == 1) {
        chosenBinding = matchingBindings[0];
    } else {
        std::vector<std::string> optionLabels;
        for (const auto& b : matchingBindings) {
            optionLabels.push_back(b->GetTrigger()->GetName()); // or use b->ToString() if better
        }

        int choiceIndex = card->Owner->PromptChooseOption(optionLabels); // see below
        if (choiceIndex < 0 || choiceIndex >= static_cast<int>(matchingBindings.size())) {
            std::cout << "❌ Invalid choice index.\n";
            return;
        }

        chosenBinding = matchingBindings[choiceIndex];
    }

    std::string chosenEvent = chosenBinding->GetTrigger()->GetName();

    std::cout << "📤 Emitting trigger: " << chosenEvent << "\n";
    card->Owner->Observer->Emit(chosenEvent, {
        { "source", card },
        { "owner", card->Owner }
    });
}



void RegisterEffectFunctions() {
    std::cout << "📦 Registering core effect functions...\n";
    EffectRegistry::Instance().Register("draw_card", DrawCard);
    EffectRegistry::Instance().Register("apply_haste", ApplyHaste);
    EffectRegistry::Instance().Register("override_deck_limit", OverrideDeckLimit);
    EffectRegistry::Instance().Register("deal_damage", DealDamage);
    EffectRegistry::Instance().Register("Aura_OverallStatBoost", Aura_OverallStatBoost);
    EffectRegistry::Instance().Register("exile_self", ExileSelf);

    EffectRegistry::Instance().Register("summon", Summon);

    EffectRegistry::Instance().Register("get_time_counter", GetTimeCounter);
    EffectRegistry::Instance().Register("remove_time_counter", RemoveTimeCounter);
    EffectRegistry::Instance().Register("choose_on_time_counter", ChooseBasedOnTimeCounter);

    std::cout << "✅ Core effect functions registered.\n";
}