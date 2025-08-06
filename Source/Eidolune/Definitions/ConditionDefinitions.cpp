#include "ConditionDefinitions.h"

bool owner_below_health(std::shared_ptr<GameCard> card, std::optional<int> value) {
    //std::cout << "Is " << card->Owner->Health << " < " << value.value() << "??\n";
    if (!card || !card->Owner || !value.has_value()) return false;
    return card->Owner->Health < value.value();
}


bool card_damaged(std::shared_ptr<GameCard> card, int value) {
    return card && card->DamageTaken > 0;
}

bool has_time_counters_equal(std::shared_ptr<GameCard> card, std::optional<int> value) {
    if (!card || !value.has_value()) return false;
    //std::cout << "[has_time_counters_equal] Card TimeCounter = " << card->TimeCounter << ", Required = " << value.value() << "\n";
    return card->TimeCounter == value.value();
}

bool damaged_enemy_hero_this_turn(std::shared_ptr<GameCard> card, std::optional<int> value) {
    if (!card || !card->Owner) return false;
    return card->Owner->DealtDamageToEnemyHeroThisTurn;
}


void RegisterConditionFunctions() {
    ConditionRegistry::Instance().RegisterFunction("owner_below_health", owner_below_health);
    ConditionRegistry::Instance().RegisterFunction("card_damaged", card_damaged);
    ConditionRegistry::Instance().RegisterFunction("has_time_counters_equal", has_time_counters_equal);
    ConditionRegistry::Instance().RegisterFunction("owner_damaged_enemy_player_this_turn", damaged_enemy_hero_this_turn);
}
