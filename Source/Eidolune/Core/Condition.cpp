

#include "Condition.h"

Condition::Condition(const std::string& name, const std::string& ref, const std::string& desc = "")
        : Name(name), ScriptReference(ref), Description(desc) {}


std::string Condition::ToString() const { return Name; }
