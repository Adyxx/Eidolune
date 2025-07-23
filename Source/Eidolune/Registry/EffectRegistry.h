
#pragma once
#include <string>
#include <unordered_map>
#include <functional>
#include <optional>

enum class TargetHint {
    CARD,
    PLAYER,
    NONE
};

struct EffectEntry {
    std::function<void(void* source, void* target, std::optional<int> value)> Func;
    TargetHint Target;
};

class EffectRegistry {
public:
    static void Register(const std::string& name, const EffectEntry& entry);
    static const EffectEntry* Get(const std::string& name);

private:
    static std::unordered_map<std::string, EffectEntry> Registry;
};
