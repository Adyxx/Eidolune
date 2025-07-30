

#include "Condition.h"

std::string Condition::ToString() const { return ScriptReference; }


Condition::Condition(const json& j) {
    ScriptReference = j["script_reference"];
    Name = j["name"];
    Description = j["description"];
}
