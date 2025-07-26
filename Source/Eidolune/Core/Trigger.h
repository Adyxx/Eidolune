#pragma once
#include <string>
#include <optional>
#include "Types.h"

class Trigger {
public:
    std::string ScriptReference;
    std::string Description;
    //std::optional<Zone> TriggerZone;

    //Trigger(const std::string& ref, const std::string& desc = "", std::optional<Zone> zone = std::nullopt);
    Trigger(const std::string& ref, const std::string& desc = "");

    std::string ToString() const;
};
