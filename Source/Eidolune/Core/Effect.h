#pragma once
#include <string>
#include <stdexcept>
#include "Types.h"
#include <optional>
#include <functional>

//#include "../External/json.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Effect {
public:
    std::string Name;
    std::string Description;
    std::string ScriptReference;
    bool RequiresValue = false;
    bool RequiresTarget = false;

    Effect(const std::string& name, const std::string& ref, bool requiresValue, bool requiresTarget, const std::string& desc = "");
    Effect(const std::string& name); 
    Effect(const json& j);

    std::string ToString() const;
    TargetHint GetTargetHint() const;

    std::function<void(void*, void*, std::optional<int>)> GetExecutable() const;

};
