#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <memory>
#include <iostream>
#include "GameCard.h"
#include "../Core/Condition.h"

class ConditionRegistry {
public:
    using ConditionFunc = std::function<bool(std::shared_ptr<GameCard>, int)>;

    static ConditionRegistry& Instance();

    // Registers full Condition object (used by loader)
    void Register(int id, const std::string& code, std::shared_ptr<Condition> condition) {
        registry[code] = condition;
        idToCondition[id] = condition;
        std::cout << "[ConditionRegistry] Registered condition: " << code << "\n";
    }

    // Registers just the logic function for later dynamic lookup
    void RegisterFunction(const std::string& code, ConditionFunc func) {
        logicFunctions[code] = func;
        std::cout << "[ConditionRegistry] Registered function logic: " << code << "\n";
    }

    std::shared_ptr<Condition> Get(const std::string& code) const {
        auto it = registry.find(code);
        return (it != registry.end()) ? it->second : nullptr;
    }

    std::shared_ptr<Condition> Get(int id) const {
        auto it = idToCondition.find(id);
        return (it != idToCondition.end()) ? it->second : nullptr;
    }

    ConditionFunc GetFunction(const std::string& code) const {
        auto it = logicFunctions.find(code);
        return (it != logicFunctions.end()) ? it->second : nullptr;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Condition>> registry;
    std::unordered_map<int, std::shared_ptr<Condition>> idToCondition;
    std::unordered_map<std::string, ConditionFunc> logicFunctions;
};
