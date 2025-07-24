#pragma once
#include <string>
#include "../Core/GameCard.h"

class ConditionEvaluator {
public:
    static bool Evaluate(const std::string& conditionCode, std::shared_ptr<GameCard> card, int value);
};
