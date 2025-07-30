#include "TriggerRegistry.h"
#include <iostream>


TriggerRegistry& TriggerRegistry::Instance() {
    static TriggerRegistry instance;
    return instance;
}
/*
void TriggerRegistry::RegisterInfo(const std::string& code, const TriggerInfo& info) {
    infoRegistry[code] = info;
}

void TriggerRegistry::RegisterShared(const std::string& code, std::shared_ptr<Trigger> trigger) {
    std::cout << "[TriggerRegistry] Registering triger: " << trigger << "\n";
    triggerRegistry[code] = trigger;
}

const std::unordered_map<std::string, TriggerInfo>& TriggerRegistry::GetAllInfo() const {
    return infoRegistry;
}

const TriggerInfo* TriggerRegistry::GetInfo(const std::string& code) const {
    auto it = infoRegistry.find(code);
    return it != infoRegistry.end() ? &it->second : nullptr;
}

std::shared_ptr<Trigger> TriggerRegistry::GetShared(const std::string& code) const {
    auto it = triggerRegistry.find(code);
    return it != triggerRegistry.end() ? it->second : nullptr;
}

void TriggerRegistry::RegisterShared(int id, std::shared_ptr<Trigger> trigger) {
    triggerRegistryById[id] = trigger;
}

std::shared_ptr<Trigger> TriggerRegistry::GetShared(int id) const {
    auto it = triggerRegistryById.find(id);
    return it != triggerRegistryById.end() ? it->second : nullptr;
}
*/