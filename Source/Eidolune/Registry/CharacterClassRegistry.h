
#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "../Classes/CharacterClass.h"
#include "Types.h"


class CharacterClassRegistry {
public:
    static CharacterClassRegistry& Instance();

    void Register(CharacterClassType type, std::shared_ptr<CharacterClass> cls);
    std::shared_ptr<CharacterClass> Resolve(CharacterClassType type) const;

private:
    CharacterClassRegistry();
    std::unordered_map<CharacterClassType, std::shared_ptr<CharacterClass>> classMap;
};
