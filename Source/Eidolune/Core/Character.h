

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include "Types.h"
#include "CharacterAbilityRegistry.h"

class Subtype;
class Faction;
class CharacterClass;

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

    int Health = 0;
    std::optional<int> PartnerHP;

    std::string ImagePath;
    std::string ModelFilePath;

    std::shared_ptr<Faction> BelongsToFaction;
    std::vector<std::shared_ptr<Subtype>> Subtypes;

    CharacterClassType Class = CharacterClassType::UNKNOWN;
    std::shared_ptr<CharacterClass> ClassLogic;

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

    CharacterAbility PassiveAbilityLogic;
    CharacterAbility ActiveAbilityLogic;

    void ResolveClass();
    void InitAbilities();
    void UsePassive();
    void UseActive();

};
