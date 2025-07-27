#include "ConditionRegistry.h"

std::unordered_map<std::string, ConditionFunction> ConditionRegistry::Registry;

void ConditionRegistry::Register(const std::string& code, ConditionFunction func) {
    Registry[code] = func;
}

const ConditionFunction* ConditionRegistry::Get(const std::string& code) {
    auto it = Registry.find(code);
    return it != Registry.end() ? &it->second : nullptr;
}
