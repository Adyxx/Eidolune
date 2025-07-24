

#include "Trigger.h"


Trigger::Trigger(const std::string& ref, const std::string& desc, std::optional<Zone> zone)
        : ScriptReference(ref), Description(desc), TriggerZone(zone) {}

std::string Trigger::ToString() const { return ScriptReference; }