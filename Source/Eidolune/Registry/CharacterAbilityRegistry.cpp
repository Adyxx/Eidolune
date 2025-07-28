
#include "CharacterAbilityRegistry.h"
#include <stdexcept>

CharacterAbilityRegistry& CharacterAbilityRegistry::Instance() {
    static CharacterAbilityRegistry instance;
    return instance;
}

void CharacterAbilityRegistry::Register(const std::string& id, const CharacterAbility& ability) {
    Abilities[id] = ability;
}

CharacterAbility CharacterAbilityRegistry::Get(const std::string& id) const {
    auto it = Abilities.find(id);
    if (it != Abilities.end()) {
        return it->second;
    }
    throw std::runtime_error("Character ability not found: " + id);
}
