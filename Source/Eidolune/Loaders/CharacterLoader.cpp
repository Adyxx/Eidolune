#include "CharacterLoader.h"
#include "../Http/ApiClient.h"
#include "../Registry/FactionRegistry.h"
#include "../Registry/CharacterAbilityRegistry.h"
#include "../Registry/CharacterClassRegistry.h"
#include "../Core/Character.h"
#include "../Registry/SubtypeRegistry.h"
#include "../Registry/CharacterRegistry.h"

#include <iostream>
#include <stdexcept>
#include <unordered_map>

static Gender ParseGender(const std::string& s) {
    if (s == "MALE") return Gender::MALE;
    if (s == "FEMALE") return Gender::FEMALE;
    return Gender::UNKNOWN;
}

static CharacterClassType ParseCharacterClassType(const std::string& s) {
    if (s == "BLOODBOUND") return CharacterClassType::BLOODBOUND;
    if (s == "GAMBLER") return CharacterClassType::GAMBLER;
    if (s == "OATHBOUND") return CharacterClassType::OATHBOUND;
    return CharacterClassType::UNKNOWN;
}

std::unordered_map<int, std::shared_ptr<Character>> CharacterLoader::LoadAll() {
    std::cout << "🔄 Loading Characters from API...\n";

    auto jsonArray = ApiClient::Get("characters/");
    std::unordered_map<int, std::shared_ptr<Character>> result;

    for (const auto& c : jsonArray) {
        if (!c.contains("id") || !c.contains("name")) {
            std::cerr << "❌ Skipping character — missing required fields: " << c.dump() << "\n";
            continue;
        }

        try {
            auto character = std::make_shared<Character>();
            int id = c["id"].get<int>();
            
            character->ID = id;
            character->Name = c.value("name", "");
            character->Age = c.value("age", 0);
            character->HeightCm = c.value("height_cm", 0.0f);
            character->GenderType = ParseGender(c.value("gender", "UNKNOWN"));

            character->BirthDate = c.value("birth_date", "");
            character->ShortBio = c.value("short_bio", "");
            character->Backstory = c.value("backstory", "");
            character->ImagePath = c.value("image_path", "");
            character->ModelFilePath = c.value("model_file_path", "");

            if (c.contains("faction_id") && c["faction_id"].is_number_integer()) {
                character->BelongsToFaction = FactionRegistry::Instance().Get(c["faction_id"]);
            }
            
            if (c.contains("subtypes") && c["subtypes"].is_array()) {
                for (const auto& subtypeId : c["subtypes"]) {
                    if (!subtypeId.is_number_integer()) continue;
                    auto subtype = SubtypeRegistry::Instance().Get(subtypeId);
                    if (subtype) {
                         character->Subtypes.push_back(subtype);
                    } else {
                        std::cerr << "⚠️ Warning: Card '" <<  character->Name << "' refers to unknown subtype ID " << subtypeId << "\n";
                    }
                }
            }
            
            std::string classStr = c.value("class_type", "UNKNOWN");
            character->Class = ParseCharacterClassType(classStr);
            character->ClassLogic = CharacterClassRegistry::Instance().Resolve(character->Class);

            character->Health = c.value("health", 0);
            if (c.contains("partner_hp") && !c["partner_hp"].is_null())
                character->PartnerHP = c["partner_hp"].get<int>();


            character->PassiveAbilityRef = c.value("passive_ability_ref", "");
            character->PassiveDescription = c.value("passive_description", "");
            character->ActiveAbilityRef = c.value("active_ability_ref", "");
            character->ActiveDescription = c.value("active_description", "");
            character->PartnerAbilityRef = c.value("partner_ability_ref", "");
            character->PartnerDescription = c.value("partner_description", "");
            character->SoloBonusRef = c.value("solo_bonus_ref", "");
            character->SoloBonusDescription = c.value("solo_bonus_description", "");
            character->DeckRestrictionRef = c.value("deck_restriction_ref", "");
            character->DeckRestrictionDescription = c.value("deck_restriction_description", "");

            character->InitAbilities();

            result[id] = character;
            CharacterRegistry::Instance().Register(id, character);

        } catch (const std::exception& e) {
            std::cerr << "❌ Error loading character: " << e.what() << " in entry: " << c.dump() << "\n";
        }
    }
    std::cout << "✅ Loaded " << result.size() << " characters.\n";
    return result;
}
