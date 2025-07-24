#pragma once
#include <string>
#include <optional>

enum class Zone {
    BOARD,
    HAND,
    DECK,
    GRAVEYARD,
    UNKNOWN
};

class Trigger {
public:
    std::string ScriptReference;
    std::string Description;
    std::optional<Zone> TriggerZone;

    Trigger(const std::string& ref, const std::string& desc = "", std::optional<Zone> zone = std::nullopt);

    std::string ToString() const;
};
