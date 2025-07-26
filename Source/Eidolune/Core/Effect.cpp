

#include "Effect.h"
#include "EffectRegistry.h"
#include <iostream>

Effect::Effect(const std::string& name) : Name(name) {}

Effect::Effect(const json& j) {
    Name = j.get<std::string>();
    ScriptReference = Name;
}

Effect::Effect(const std::string& name,
               const std::string& ref,
               bool requiresValue,
               bool requiresTarget,
               const std::string& desc)
    : Name(name),
      Description(desc),
      ScriptReference(ref),
      RequiresValue(requiresValue),
      RequiresTarget(requiresTarget) {}


std::string Effect::ToString() const {
    return Name;
}


std::function<void(void*, void*, std::optional<int>)> Effect::GetExecutable() const {
    const EffectEntry* entry = EffectRegistry::Get(this->ScriptReference);
    if (!entry) {
        std::cout << "Effect script_reference not found: " << ScriptReference << "\n";
        throw std::runtime_error("Effect script_reference not found: " + ScriptReference);
    }
    return entry->Func;
}

TargetHint Effect::GetTargetHint() const {
    const EffectEntry* entry = EffectRegistry::Get(ScriptReference);
    return entry ? entry->Target : TargetHint::NONE;
}