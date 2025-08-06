#pragma once
#include <string>
#include <optional>
#include "Types.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Trigger {
public:
    std::string ScriptReference;
    std::string Description;
    std::string Event;

    std::string ToString() const;
    Trigger(const json& j);

    std::string GetName() const;

};