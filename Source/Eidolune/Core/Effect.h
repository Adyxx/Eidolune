#pragma once
#include <string>
#include <stdexcept>
#include <optional>
#include <functional>

#include "Types.h"
//#include "Target.h"

struct Target;

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Effect {
public:
    std::string Name;
    std::string Description;
    std::string ScriptReference;
    bool RequiresValue = false;
    bool RequiresTarget = false;
    TargetHint targetInfo;
    std::function<void(void*, Target, std::optional<int>)> Executable;

    Effect(const std::string& name, const std::string& ref, bool requiresValue, bool requiresTarget, const std::string& desc = "");
    
    Effect(const std::string& name, std::function<void(void*, Target, std::optional<int>)> func)
        : ScriptReference(name), Executable(std::move(func)) {}

    Effect(const std::string& name); 
    Effect(const json& j);

    std::string ToString() const;
    TargetHint GetTargetHint() const;

    std::function<void(void*, Target, std::optional<int>)> GetExecutable() const;
};