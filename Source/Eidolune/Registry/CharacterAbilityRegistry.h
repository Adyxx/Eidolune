#pragma once
#include <unordered_map>
#include <string>
#include <functional>

//#include "../Core/Character.h"

class Character;
#include "../Core/GameState.h"

using CharacterAbilityFn = std::function<void(Character& self, GameState& state)>;

struct CharacterAbility {
    CharacterAbilityFn Logic;
    std::string Description;
};

class CharacterAbilityRegistry {
public:
    static CharacterAbilityRegistry& Instance();

    void Register(const std::string& id, const CharacterAbility& ability);
    CharacterAbility Get(const std::string& id) const;

private:
    std::unordered_map<std::string, CharacterAbility> Abilities;
};