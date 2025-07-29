#pragma once

#include <unordered_map>
#include <memory>
#include "../Core/Character.h"

class CharacterRegistry {
public:
    static CharacterRegistry& Instance();

    void Register(int id, std::shared_ptr<Character> character);
    std::shared_ptr<Character> Get(int id) const;

    const std::unordered_map<int, std::shared_ptr<Character>>& GetAll() const {
        return Characters;
    }

private:
    std::unordered_map<int, std::shared_ptr<Character>> Characters;

    CharacterRegistry() = default;
    CharacterRegistry(const CharacterRegistry&) = delete;
    CharacterRegistry& operator=(const CharacterRegistry&) = delete;
};
