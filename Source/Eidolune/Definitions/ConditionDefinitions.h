#pragma once
#include <memory>
#include "GameCard.h"
#include "ConditionRegistry.h"
#include "Player.h"

bool owner_below_10_health(std::shared_ptr<GameCard> card, int value);
bool card_damaged(std::shared_ptr<GameCard> card, int value);
void RegisterConditionFunctions();
/*
bool played_more_than_n_cards_this_turn(std::shared_ptr<GameCard> card, int n) {
    if (!card || !card->Owner) return false;
    return card->Owner->CardsPlayedThisTurn > n;
}

bool damaged_enemy_hero_this_turn(std::shared_ptr<GameCard> card, int) {
    
    if (!card || !card->Owner) return false;
    Player* enemy = card->Owner->GetOpponent();
    return GameState::Instance().HeroDamagedThisTurn[enemy->ID];
    
}
*/