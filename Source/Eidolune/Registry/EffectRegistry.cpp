
#include "EffectRegistry.h"

std::unordered_map<std::string, EffectEntry> EffectRegistry::Registry;

void EffectRegistry::Register(const std::string& name, const EffectEntry& entry) {
    Registry[name] = entry;
}

const EffectEntry* EffectRegistry::Get(const std::string& name) {
    auto it = Registry.find(name);
    return it != Registry.end() ? &it->second : nullptr;
}
