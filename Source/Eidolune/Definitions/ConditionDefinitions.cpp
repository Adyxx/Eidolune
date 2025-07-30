#include "ConditionDefinitions.h"

bool owner_below_10_health(std::shared_ptr<GameCard> card, int value) {
    if (!card || !card->Owner) return false;
    return card->Owner->Health < 10;
}

bool card_damaged(std::shared_ptr<GameCard> card, int value) {
    return card && card->DamageTaken > 0;
}

void RegisterConditionFunctions() {
    ConditionRegistry::Instance().RegisterFunction("owner_below_10_health", owner_below_10_health);
    ConditionRegistry::Instance().RegisterFunction("card_damaged", card_damaged);
}
