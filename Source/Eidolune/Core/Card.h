#pragma once
#include <string>
#include <memory>
#include <vector>
#include <optional>
#include "Player.h"
#include "Types.h"

class Faction;
class Character;
class Subtype;
class CardEffectBinding;


class Card {
public:
    int ID = -1;
    std::string Name;
    std::string ImagePath;
    int Cost;
    Rarity CardRarity;
    CardType Type;

    std::shared_ptr<Faction> CardFaction;
    std::vector<std::shared_ptr<Subtype>> Subtypes;

    std::optional<int> Power;
    std::optional<int> Health;

    bool IsCharacterCard = false;
    bool IsCharacterExclusive = false;
    std::shared_ptr<Character> CharacterRef;

    std::string Text;
    std::vector<std::shared_ptr<CardEffectBinding>> EffectBindings;

    CharacterClassType ClassLock = CharacterClassType::UNKNOWN;
    AuxiliaryCardType AuxilaryType = AuxiliaryCardType::NONE;

    Card();
    void Validate() const;
    std::string ToString() const;
    bool HasSubtype(const std::string& targetSubtype) const;
    
    inline bool IsConsumable() const {
        return AuxilaryType == AuxiliaryCardType::CONSUMABLE || AuxilaryType == AuxiliaryCardType::REAGENT;
    }

    bool IsPullable() const {
        if (AuxilaryType != AuxiliaryCardType::NONE) return false;
        if (CardRarity == Rarity::COMMON && Name == "Basic potion") return false;
        // Add any other disqualifying logic
        return true;
    }


};
