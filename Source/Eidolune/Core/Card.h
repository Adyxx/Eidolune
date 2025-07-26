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

    Card();
    void Validate() const;
    std::string ToString() const;
};
