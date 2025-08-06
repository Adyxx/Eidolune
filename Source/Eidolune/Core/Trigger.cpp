

#include "Trigger.h"

std::string Trigger::ToString() const { return ScriptReference; }

Trigger::Trigger(const json& j) {
    ScriptReference = j["script_reference"];
    Description = j["description"];
    Event = j.contains("event") ? j["event"] : "";
}


std::string Trigger::GetName() const {
    return ScriptReference;
}