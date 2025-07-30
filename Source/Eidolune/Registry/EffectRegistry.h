#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <optional>
#include "Types.h"
#include "Target.h"
#include "../Core/Effect.h"

#include <iostream>
#include <memory>

struct EffectEntry {
    std::function<void(void*, Target, std::optional<int>)> Func;
    TargetHint Target;
};

class EffectRegistry {
public:
    using EffectFunction = std::function<void(void*, Target, std::optional<int>)>;

    static EffectRegistry& Instance();

    // 🔷 Register full Effect object with ID
    void Register(int id, const std::string& name, std::shared_ptr<Effect> effect) {
        registry[name] = effect;
        idToEffect[id] = effect;
        std::cout << "[EffectRegistry] Registering effect by ID: " << name << "\n";
    }

    // 🔷 Register basic function by name (usually from EffectDefinitions)
    void Register(const std::string& name, EffectFunction func) {
        registry[name] = std::make_shared<Effect>(name, func);
        std::cout << "[EffectRegistry] Registered effect function: " << name << "\n";
    }

    std::shared_ptr<Effect> Get(const std::string& name) const {
        auto it = registry.find(name);
        return (it != registry.end()) ? it->second : nullptr;
    }

    std::shared_ptr<Effect> Get(int id) const {
        auto it = idToEffect.find(id);
        return (it != idToEffect.end()) ? it->second : nullptr;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Effect>> registry;
    std::unordered_map<int, std::shared_ptr<Effect>> idToEffect;
};
