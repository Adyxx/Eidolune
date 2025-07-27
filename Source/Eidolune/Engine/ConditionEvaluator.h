#pragma once
#include <string>
#include <memory>
#include "GameCard.h"
#include "ConditionRegistry.h"

class ConditionEvaluator {
public:
    static bool Evaluate(const std::string& code, std::shared_ptr<GameCard> card, int value);
};
