#include <iostream>

#include "EffectDefinitions.h"
#include "EffectRegistry.h"
#include "Player.h"
#include "GameCard.h"
#include "TriggerObserver.h"
#include "GameActions.h"
#include "CardUtils.h"
#include "EffectContext.h"
#include "Card.h"
#include "Subtype.h"
#include "EffectHelpers.h"

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

void ExileSelf(void* source, Target, std::optional<int>) {
    auto* card = static_cast<GameCard*>(source);
    if (!card || !card->Owner) return;

    auto& player = card->Owner;

    bool moved = EffectHelpers::MoveCardToZone(card, player, player->Hand, player->ExileZone, "Hand", "Exile")
              || EffectHelpers::MoveCardToZone(card, player, player->Board, player->ExileZone, "Board", "Exile")
              || EffectHelpers::MoveCardToZone(card, player, player->Graveyard, player->ExileZone, "Graveyard", "Exile")
              || EffectHelpers::MoveCardToZone(card, player, player->DrawPile, player->ExileZone, "DrawPile", "Exile")
              || EffectHelpers::MoveCardToZone(card, player, player->OathZone, player->ExileZone, "OathZone", "Exile");

    if (!moved) {
        std::cout << "⚠️ Could not find card in any zone.\n";
    }

    card->Zone = CardZone::EXILE;
}

void Aura_OverallStatBoost(void* source, Target, std::optional<int> value) {
    if (!source) return;

    auto* sourceCard = static_cast<GameCard*>(source);
    if (!sourceCard || !sourceCard->Owner) return;

    int buffAmount = value.value_or(1);

    for (int row = 0; row < Player::BoardHeight; ++row) {
        for (int col = 0; col < Player::BoardWidth; ++col) {
            auto& slot = sourceCard->Owner->GridBoard[row][col];
            if (!slot) continue;

            auto& card = slot;

            // Avoid reapplying the same aura multiple times
            if (card->ActiveAuras.count(sourceCard->Id)) {
                continue;
            }

            card->ApplyAura(sourceCard->Id, buffAmount, buffAmount);

            std::cout << "🟢 Aura from [" << sourceCard->GetName()
                      << "] applied to [" << card->GetName()
                      << "] (+" << buffAmount << "/+" << buffAmount << ")\n";
        }
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

        int choiceIndex = card->Owner->PromptChooseOption(optionLabels);
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


void SearchDeck(void* source, Target target, std::optional<int> value) {
    auto binding = GetEffectCallContext().CurrentBinding;
    if (!binding) return;

    auto* player = static_cast<Player*>(target.ptr);
    if (!player) return;

    int fetchCount = value.value_or(1);

    std::vector<std::shared_ptr<GameCard>> validCards;

    // If template linked, filter based on its metadata
    if (binding->LinkedCard && binding->LinkedCard->AuxiliaryType == AuxiliaryCardType::TEMPLATE) {
        auto* templateCard = binding->LinkedCard.get();
        for (auto& card : player->DrawPile) {
            if (templateCard->Type != CardType::UNKNOWN &&
                card->Model->Type != templateCard->Type) continue;

            if (!templateCard->Subtypes.empty()) {
                bool hasSubtype = false;
                for (auto& sub : templateCard->Subtypes) {
                    if (card->Model->HasSubtype(sub->Name)) { hasSubtype = true; break; }
                }
                if (!hasSubtype) continue;
            }

            if (templateCard->Cost >= 0 && card->Model->Cost > templateCard->Cost) continue;


            validCards.push_back(card);
        }
    }
    else {
        // No template — any card is valid
        validCards = player->DrawPile;
    }

    if (validCards.empty()) {
        std::cout << "❌ No valid cards to search for.\n";
        return;
    }

    // Let player pick if more than one
    std::vector<std::string> names;
    for (auto& c : validCards) names.push_back(c->GetName());

    int choiceIndex = 0;
    if (validCards.size() > 1)
        choiceIndex = player->PromptChooseOption(names);

    if (choiceIndex < 0 || choiceIndex >= (int)validCards.size()) return;

    auto chosenCard = validCards[choiceIndex];

    // Remove from deck & put into hand
    auto it = std::find(player->DrawPile.begin(), player->DrawPile.end(), chosenCard);
    if (it != player->DrawPile.end()) player->DrawPile.erase(it);

    player->Hand.push_back(chosenCard);
    chosenCard->Zone = CardZone::HAND;

    std::cout << "🔍 Searched and added: " << chosenCard->GetName() << " to hand.\n";
}

void DestroyTarget(void* source, Target target, std::optional<int>) {
    if (target.type != Target::Type::CARD) return;
    auto* card = static_cast<GameCard*>(target.ptr);
    if (!card || !card->Owner) return;

    if (card->Owner->RemoveCardFromBoard(card)) {
        std::cout << "💀 Destroyed '" << card->GetName() << "' and sent to graveyard.\n";
    } else {
        std::cout << "⚠️ DestroyTarget: Card not found on board.\n";
    }
}

void ReturnToHand(void* source, Target target, std::optional<int>) {
    if (target.type != Target::Type::CARD) return;
    auto* card = static_cast<GameCard*>(target.ptr);
    if (!card || !card->Owner) return;

    Player* owner = card->Owner;

    // Remove from board
    for (int r = 0; r < Player::BoardHeight; ++r) {
        for (int c = 0; c < Player::BoardWidth; ++c) {
            if (owner->GridBoard[r][c] && owner->GridBoard[r][c].get() == card) {
                owner->Hand.push_back(owner->GridBoard[r][c]);
                owner->GridBoard[r][c] = nullptr;
                card->Zone = CardZone::HAND;
                std::cout << "↩️ Returned '" << card->GetName() << "' to hand.\n";
                return;
            }
        }
    }

    std::cout << "⚠️ ReturnToHand: Card not found on board.\n";
}

void ReviveFromGraveyard(void* source, Target target, std::optional<int>) {
    auto* player = static_cast<Player*>(target.ptr);
    if (!player) return;

    if (player->Graveyard.empty()) {
        std::cout << "⚠️ No cards in graveyard to revive.\n";
        return;
    }

    // Let player choose from graveyard
    std::vector<std::string> names;
    for (auto& c : player->Graveyard) names.push_back(c->GetName());

    int choiceIndex = player->PromptChooseOption(names);
    if (choiceIndex < 0 || choiceIndex >= (int)player->Graveyard.size()) return;

    auto chosenCard = player->Graveyard[choiceIndex];
    auto freePos = player->GetRandomFreeBoardSlot();

    if (!freePos) {
        std::cout << "🚫 No free board slot to revive card.\n";
        return;
    }

    Position pos = *freePos;
    player->GridBoard[pos.row][pos.col] = chosenCard;
    chosenCard->Zone = CardZone::BOARD;

    // Remove from graveyard
    player->Graveyard.erase(player->Graveyard.begin() + choiceIndex);

    std::cout << "❤️‍🩹 Revived '" << chosenCard->GetName() << "' at ("
              << pos.row << "," << pos.col << ").\n";
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
    EffectRegistry::Instance().Register("search_deck", SearchDeck);

    EffectRegistry::Instance().Register("get_time_counter", GetTimeCounter);
    EffectRegistry::Instance().Register("remove_time_counter", RemoveTimeCounter);
    EffectRegistry::Instance().Register("choose_on_time_counter", ChooseBasedOnTimeCounter);

    // not tested ....
    EffectRegistry::Instance().Register("destroy_target", DestroyTarget);
    EffectRegistry::Instance().Register("return_to_hand", ReturnToHand);
    EffectRegistry::Instance().Register("revive_from_graveyard", ReviveFromGraveyard);


    std::cout << "✅ Core effect functions registered.\n";
}