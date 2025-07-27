#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include "GameCard.h"
#include "Player.h"

using ConditionFunction = std::function<bool(std::shared_ptr<GameCard>, int)>;

class ConditionRegistry {
public:
    static void Register(const std::string& code, ConditionFunction func);
    static const ConditionFunction* Get(const std::string& code);
private:
    static std::unordered_map<std::string, ConditionFunction> Registry;
};
