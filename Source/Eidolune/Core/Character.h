

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "Subtype.h"
#include "Faction.h"
#include "Types.h"
#include "CharacterClassRegistry.h"
#include "../Registry/CharacterAbilityRegistry.h"

class Character {
public:
    int ID = -1;
    std::string Name;
    int Age = 0;
    float HeightCm = 0.0f;
    Gender GenderType = Gender::UNKNOWN;
    
    std::string BirthDate;
    std::string ShortBio;
    std::string Backstory;

    std::string ImagePath;
    std::string ModelFilePath;

    std::shared_ptr<Faction> BelongsToFaction;
    std::vector<std::shared_ptr<Subtype>> Subtypes;

    CharacterClassType Class = CharacterClassType::UNKNOWN;
    std::shared_ptr<CharacterClass> ClassLogic;

    int Health = 0;
    std::optional<int> PartnerHP;

    std::string PassiveAbilityRef;
    std::string PassiveDescription;

    std::string ActiveAbilityRef;
    std::string ActiveDescription;

    std::string PartnerAbilityRef;
    std::string PartnerDescription;

    std::string SoloBonusRef;
    std::string SoloBonusDescription;

    std::string DeckRestrictionRef;
    std::string DeckRestrictionDescription;

    std::string ToString() const;

    void ResolveClass() {
        ClassLogic = CharacterClassRegistry::Instance().Resolve(Class);
    }

    CharacterAbility PassiveAbilityLogic;
    CharacterAbility ActiveAbilityLogic;

    void InitAbilities() {
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

    void UsePassive(GameState& state) {
    if (PassiveAbilityLogic.Logic)
        PassiveAbilityLogic.Logic(*this, state);
    }

    void UseActive(GameState& state) {
    if (ActiveAbilityLogic.Logic)
        ActiveAbilityLogic.Logic(*this, state);
    }


};
