
#include "ConditionEvaluator.h"
#include <iostream>

bool ConditionEvaluator::Evaluate(const std::string& code, std::shared_ptr<GameCard> card, int value) {
    std::cout << "🔍 Evaluating condition: " << code << "\n";
    auto funcPtr = ConditionRegistry::Get(code);
    if (!funcPtr) {
        std::cout << "⚠️ Unknown condition code: " << code << "\n";
        return false;
    }
    return (*funcPtr)(card, value);
}
