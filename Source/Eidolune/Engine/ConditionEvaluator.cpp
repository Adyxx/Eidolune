#include "ConditionEvaluator.h"
#include "ConditionRegistry.h"
#include <iostream>

bool ConditionEvaluator::Evaluate(const std::string& code, std::shared_ptr<GameCard> card, int value) {
    //std::cout << "🔍 Evaluating condition: " << code << "\n";

    auto func = ConditionRegistry::Instance().GetFunction(code);
    if (!func) {
        std::cerr << "⚠️ No logic function registered for condition: " << code << "\n";
        return false;
    }

    return func(card, value);
}
