#include "ConditionRegistry.h"
#include "ConditionDefinitions.h"
#include "../Core/GameCard.h"
#include "../Core/Player.h"

void RegisterConditionDefinitions() {
    ConditionRegistry::Register("owner_below_10_health", [](std::shared_ptr<GameCard> card, int value) {
        if (!card || !card->Owner) return false;
        return card->Owner->Health < 10;
    });

    ConditionRegistry::Register("card_damaged", [](std::shared_ptr<GameCard> card, int value) {
        return card && card->DamageTaken > 0;
    });


}
