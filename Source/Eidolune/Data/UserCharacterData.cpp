#include "UserCharacterData.h"
#include "../Core/Character.h"
#include "../Core/Card.h"
#include "../Classes/SpecialistClass.h"

nlohmann::json UserCharacterData::ToJson() const {
    return {
        {"character_id", CharacterId},
        {"level", Level},
        {"equipped", EquippedItems},
        {"consumables", Consumables}
    };
}

UserCharacterData UserCharacterData::FromJson(const nlohmann::json& j) {
    UserCharacterData data;
    data.CharacterId = j.at("character_id").get<int>();
    data.Level = j.value("level", 1);
    data.EquippedItems = j.value("equipped", std::unordered_map<std::string, std::string>{});
    data.Consumables = j.value("consumables", std::vector<int>{});
    return data;
}

UserCharacterData UserCharacterData::FromCharacter(Character* character) {
    UserCharacterData data;
    if (!character) return data;

    data.CharacterId = character->ID;
    data.EquippedItems.clear();

    if (character->Class == CharacterClassType::SPECIALIST) {
        auto* specialist = static_cast<SpecialistClass*>(character->ClassLogic.get());
        if (specialist) {
            const auto& bag = specialist->GetBag(character);
            for (const auto& slot : bag.slots) {
                if (slot.item && slot.item->SourceCard) {
                    data.Consumables.push_back(slot.item->SourceCard->ID);
                } else {
                    data.Consumables.push_back(-1);
                }
            }
        }
    }

    return data;
}


bool UserCharacterData::IsCharacterOwned(int charId, const std::vector<UserCharacterData>& ownedChars) {
    return std::any_of(ownedChars.begin(), ownedChars.end(),
        [charId](const UserCharacterData& data) {
            return data.CharacterId == charId;
        });
}
