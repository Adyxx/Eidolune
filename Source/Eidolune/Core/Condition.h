#pragma once
#include <string>
#include <functional>
#include "GameCard.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

using ConditionFunction = std::function<bool(std::shared_ptr<GameCard>, int)>;

class Condition {
public:
    std::string Name;
    std::string ScriptReference;
    ConditionFunction Func;

    std::string Description;


    Condition(const std::string& name, const std::string& ref, ConditionFunction func, const std::string& desc = "")
        : Name(name), ScriptReference(ref), Func(func), Description(desc) {}

    std::string ToString() const;
    Condition(const json& j);
};
