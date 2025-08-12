
#include "Character.h"
#include "CharacterClassRegistry.h"

std::string Character::ToString() const { return Name; }

void Character::ResolveClass() { ClassLogic = CharacterClassRegistry::Instance().Resolve(Class);}

void Character::InitAbilities() {
    if (!PassiveAbilityRef.empty()) {
        PassiveAbilityLogic = CharacterAbilityRegistry::Instance().Get(PassiveAbilityRef);
    } else {
        PassiveAbilityLogic = CharacterAbility{};
    }

    if (!ActiveAbilityRef.empty()) {
        ActiveAbilityLogic = CharacterAbilityRegistry::Instance().Get(ActiveAbilityRef);
    } else {
        ActiveAbilityLogic = CharacterAbility{};
    }
}

void Character::UsePassive() {

}

void Character::UseActive() {

}
