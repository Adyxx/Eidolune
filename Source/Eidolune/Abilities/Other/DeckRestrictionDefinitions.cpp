
#include "DeckRestrictionDefinitions.h"
#include "../Registry/DeckRestrictionRegistry.h"
#include <algorithm>
#include "DeckCard.h"
#include "Card.h"


void RegisterDeckRestrictions() {
    DeckRestrictionRegistry::Instance().Register("only_cost_5_or_less", {
        [](const Deck& deck) {
            return std::all_of(deck.DeckCards.begin(), deck.DeckCards.end(), [](const std::shared_ptr<DeckCard>& deckCard) {
                return deckCard->CardRef && deckCard->CardRef->Cost <= 5;
            });
        },
        "Only cards with cost 5 or less"
    });
}

