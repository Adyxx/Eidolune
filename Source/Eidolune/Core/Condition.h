#pragma once
#include <string>

class Condition {
public:
    std::string Name;
    std::string ScriptReference;
    std::string Description;

    Condition(const std::string& name, const std::string& ref, const std::string& desc = "")
        : Name(name), ScriptReference(ref), Description(desc) {}

    std::string ToString() const { return Name; }
};
