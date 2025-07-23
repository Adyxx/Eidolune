#include "TriggerRegistry.h"

std::unordered_map<std::string, TriggerInfo> TriggerRegistry::Registry;

void TriggerRegistry::Register(const std::string& code, const TriggerInfo& info) {
    Registry[code] = info;
}

const std::unordered_map<std::string, TriggerInfo>& TriggerRegistry::GetAll() {
    return Registry;
}

const TriggerInfo* TriggerRegistry::Get(const std::string& code) {
    auto it = Registry.find(code);
    return it != Registry.end() ? &it->second : nullptr;
}
