
#include "EffectRegistry.h"
#include <iostream>

std::unordered_map<std::string, EffectEntry> EffectRegistry::Registry;

void EffectRegistry::Register(const std::string& name, const EffectEntry& entry) {
    std::cout << "[EffectRegistry] Registering effect: " << name << "\n";
    Registry[name] = entry;
}

const EffectEntry* EffectRegistry::Get(const std::string& name) {
    auto it = Registry.find(name);
    return it != Registry.end() ? &it->second : nullptr;
}
