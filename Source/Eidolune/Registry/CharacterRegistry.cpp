#include "CharacterRegistry.h"

CharacterRegistry& CharacterRegistry::Instance() {
    static CharacterRegistry instance;
    return instance;
}

void CharacterRegistry::Register(int id, std::shared_ptr<Character> character) {
    if (character) {
        Characters[id] = character;
    }
}

std::shared_ptr<Character> CharacterRegistry::Get(int id) const {
    auto it = Characters.find(id);
    if (it != Characters.end()) {
        return it->second;
    }
    return nullptr;
}
