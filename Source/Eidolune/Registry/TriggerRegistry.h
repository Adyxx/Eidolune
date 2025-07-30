#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "../Core/Trigger.h"
#include <functional>
#include <iostream>
#include <memory>


struct TriggerInfo {
    std::string Description;
    std::string Event;
    std::function<void()> Builder;
    std::string Name;
};

class TriggerRegistry {
public:
    static TriggerRegistry& Instance();

    void RegisterShared(const std::string& code, std::shared_ptr<Trigger> trigger) {
        triggerRegistry[code] = trigger;
        
    }

    void RegisterShared(int id, std::shared_ptr<Trigger> trigger) {
        idToTrigger[id] = trigger;
        std::cout << "[TriggerRegistry] Registering trigger: " << trigger->ToString() << "\n";
    }

    void RegisterInfo(const std::string& code, const TriggerInfo& info) {
        infoRegistry[code] = info;
    }

    std::shared_ptr<Trigger> GetShared(const std::string& code) const {
        auto it = triggerRegistry.find(code);
        return (it != triggerRegistry.end()) ? it->second : nullptr;
    }

    std::shared_ptr<Trigger> GetShared(int id) const {
        auto it = idToTrigger.find(id);
        return (it != idToTrigger.end()) ? it->second : nullptr;
    }

    const TriggerInfo* GetInfo(const std::string& code) const {
    auto it = infoRegistry.find(code);
    return it != infoRegistry.end() ? &it->second : nullptr;


}

private:
    std::unordered_map<std::string, std::shared_ptr<Trigger>> triggerRegistry;
    std::unordered_map<int, std::shared_ptr<Trigger>> idToTrigger;
    std::unordered_map<std::string, TriggerInfo> infoRegistry;
};


