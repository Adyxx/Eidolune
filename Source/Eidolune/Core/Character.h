

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <optional>

class Subtype {
public:
    std::string Name;
    Subtype(const std::string& name) : Name(name) {}
};

class Faction {
public:
    std::string Name;
    std::string Color;
    Faction(const std::string& name, const std::string& color = "") : Name(name), Color(color) {}
};

class Character {
public:
    std::string Name;
    int Age = 0;
    float HeightCm = 0.0f;
    std::string BirthDate;
    std::string ShortBio;
    std::string Backstory;

    std::string ImagePath;
    std::string ModelFilePath;

    std::shared_ptr<Faction> BelongsToFaction;
    std::vector<std::shared_ptr<Subtype>> Subtypes;

    std::string ClassType;

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

    std::string ToString() const { return Name; }
};
