

// draw, discard, deck manipulation, search deck, etc.

#include "Target.h"
#include <optional>
#include "Player.h"
#include "EffectContext.h"
#include "CardEffectBinding.h"
#include "Subtype.h"
#include "EffectRegistry.h"

void DrawCard(void* source, Target target, std::optional<int> value) {
    std::cout << "Hello from DrawCard func\n";
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



void OverrideDeckLimit(void* source, Target  target, std::optional<int> value) {
    // this is on-deck-build effect and is implemented elsewhere.
}


void SearchDeck(void* source, Target target, std::optional<int> value) {

    auto binding = GetEffectCallContext().CurrentBinding;
    if (!binding) return;

    auto* player = static_cast<Player*>(target.ptr);
    if (!player) return;

    //int fetchCount = value.value_or(1);

    std::vector<std::shared_ptr<GameCard>> validCards;

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



void RegisterEffectFunctions_Deck() {
    std::cout << "📦 Registering deck effects...\n";
    EffectRegistry::Instance().Register("draw_card", DrawCard);
    EffectRegistry::Instance().Register("search_deck", SearchDeck);
    EffectRegistry::Instance().Register("override_deck_limit", OverrideDeckLimit);
}