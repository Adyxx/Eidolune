#pragma once
#include <string>

class Effect {
public:
    std::string Name;
    std::string Description;
    std::string ScriptReference;
    bool RequiresValue = false;
    bool RequiresTarget = false;

    Effect(const std::string& name, const std::string& ref, bool requiresValue, bool requiresTarget, const std::string& desc = "")
        : Name(name), ScriptReference(ref), RequiresValue(requiresValue), RequiresTarget(requiresTarget), Description(desc) {}

    std::string ToString() const { return Name; }
};
