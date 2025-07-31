#include "DeckRestrictionDefinitions.h"
#include "DeckRestrictionRegistry.h"
#include <unordered_map>
#include "../Core/DeckCard.h"
#include "../Core/Card.h"

void RegisterDeckRestrictions() {
    DeckRestrictionRegistry::Register("at_least_10_pirates", [](std::shared_ptr<Deck> deck) {
        int count = 0;
        for (auto& dc : deck->DeckCards) {
            if (dc && dc->CardRef && dc->CardRef->HasSubtype("PIRATE")) {
                count += dc->Quantity;
            }
        }
        if (count < 10) return std::make_pair(false, "Deck must contain at least 10 PIRATE cards.");
        return std::make_pair(true, "");
    });


    DeckRestrictionRegistry::Register("only_cost_4_or_less", [](std::shared_ptr<Deck> deck) {
        for (auto& dc : deck->DeckCards) {
            if (dc && dc->CardRef && dc->CardRef->Cost > 4) {
                return std::make_pair(false, "All cards must cost 4 or less.");
            }
        }
        return std::make_pair(true, "");
    });

}
