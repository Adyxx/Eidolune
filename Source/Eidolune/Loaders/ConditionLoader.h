#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "../Registry/ConditionRegistry.h"

class Condition;

class ConditionLoader {
public:
    static std::unordered_map<std::string, std::shared_ptr<Condition>> LoadAll();
};
