

#include "ConditionEvaluator.h"
#include <iostream>

bool ConditionEvaluator::Evaluate(const std::string& conditionCode, std::shared_ptr<GameCard> card, int value) {
    if (conditionCode == "has_power") {
        return card->GetPower().value_or(0) >= value;
    }

    std::cout << "⚠️ Unknown condition: " << conditionCode << "\n";
    return false;
}
