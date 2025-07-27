#include "CharacterClassRegistry.h"

#include "../Classes/SpecialistClass.h"
#include "../Classes/SpellweaverClass.h"
#include "../Classes/BloodboundClass.h"
#include "../Classes/ShadeClass.h"
#include "../Classes/MachinistClass.h"
#include "../Classes/SoulforgedClass.h"
#include "../Classes/GamblerClass.h"
#include "../Classes/OathboundClass.h"

CharacterClassRegistry& CharacterClassRegistry::Instance() {
    static CharacterClassRegistry instance;
    return instance;
}

CharacterClassRegistry::CharacterClassRegistry() {
    Register(CharacterClassType::SPECIALIST, std::make_shared<SpecialistClass>());
    Register(CharacterClassType::SPELLWEAVER, std::make_shared<SpellweaverClass>());
    Register(CharacterClassType::BLOODBOUND, std::make_shared<BloodboundClass>());
    Register(CharacterClassType::SHADE, std::make_shared<ShadeClass>());
    Register(CharacterClassType::MACHINIST, std::make_shared<MachinistClass>());
    Register(CharacterClassType::SOULFORGED, std::make_shared<SoulforgedClass>());
    Register(CharacterClassType::GAMBLER, std::make_shared<GamblerClass>());
    Register(CharacterClassType::OATHBOUND, std::make_shared<OathboundClass>());
}

void CharacterClassRegistry::Register(CharacterClassType type, std::shared_ptr<CharacterClass> cls) {
    classMap[type] = cls;
}

std::shared_ptr<CharacterClass> CharacterClassRegistry::Resolve(CharacterClassType type) const {
    auto it = classMap.find(type);
    if (it != classMap.end()) {
        return it->second;
    }

    return nullptr;
}
