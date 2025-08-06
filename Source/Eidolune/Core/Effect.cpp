

#include "Effect.h"
#include "../Registry/EffectRegistry.h"
#include <iostream>

Effect::Effect(const std::string& name) : Name(name) {}

Effect::Effect(const json& j) {
    Name = j.value("name", "Unnamed Effect");
    ScriptReference = j.value("script_reference", Name);
    Description = j.value("description", "");
    RequiresValue = j.value("requires_value", false);
    RequiresTarget = j.value("requires_target", false);
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
    return Name.empty() ? "<UnnamedEffect>" : Name;
}


std::function<void(void*, Target, std::optional<int>)> Effect::GetExecutable() const {

    if (!Executable) {
        std::cout << "⚠️ [Effect::GetExecutable] Executable is NULL for: " << ScriptReference << "\n";
    }
    return Executable;

}


TargetHint Effect::GetTargetHint() const {
    auto effectPtr = EffectRegistry::Instance().Get(ScriptReference);
    return effectPtr ? effectPtr->GetTargetHint() : TargetHint::NONE;
}

bool Effect::RequiresLinkedCard() const {
    std::cout << "asdasdasdnxncdjkncxyknyyxccyx\n";
    return ScriptReference == "summon"; 
}