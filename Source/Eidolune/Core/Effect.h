#pragma once
#include <string>
#include <stdexcept>
#include "Types.h"

class Effect {
public:
    std::string Name;
    std::string Description;
    std::string ScriptReference;
    bool RequiresValue = false;
    bool RequiresTarget = false;

    Effect(const std::string& name, const std::string& ref, bool requiresValue, bool requiresTarget, const std::string& desc = "");

    std::string ToString() const;
    TargetHint GetTargetHint() const;

    std::function<void(void*, void*, std::optional<int>)> GetExecutable() const;

 

};
