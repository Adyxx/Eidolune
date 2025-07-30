#pragma once
#include <memory>
#include "GameCard.h"
#include "ConditionRegistry.h"


bool owner_below_10_health(std::shared_ptr<GameCard> card, int value);
bool card_damaged(std::shared_ptr<GameCard> card, int value);
void RegisterConditionFunctions();