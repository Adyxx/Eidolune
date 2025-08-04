#include "CardLoader.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "../Http/ApiClient.h" 
#include "../Core/Trigger.h"
#include "../Core/Effect.h"
#include "../Core/Condition.h"
#include "../Core/CardEffectBinding.h"
#include "../Registry/CardRegistry.h"
#include "../Registry/CharacterRegistry.h"
#include "../Registry/SubtypeRegistry.h"

static Rarity ParseRarity(const std::string& s) {
    if (s == "COMMON") return Rarity::COMMON;
    if (s == "RARE") return Rarity::RARE;
    if (s == "EPIC") return Rarity::EPIC;
    if (s == "LEGENDARY") return Rarity::LEGENDARY;
    throw std::runtime_error("Unknown rarity: " + s);
}

static CardType ParseCardType(const std::string& s) {
    if (s == "UNIT") return CardType::UNIT;
    if (s == "SPELL") return CardType::SPELL;
    if (s == "RITUAL") return CardType::RITUAL;
    if (s == "ASSET") return CardType::ASSET;
    if (s == "OTHER") return CardType::OTHER;
    throw std::runtime_error("Unknown card type: " + s);
}
/*
static TargetSpec ParseTargetSpec(const std::string& s) {
    if (s == "ENEMY_BOARD") return TargetSpec::ENEMY_BOARD;
    if (s == "ENEMY_HERO") return TargetSpec::ENEMY_HERO;
    if (s == "ENEMY_BOARD_OR_HERO") return TargetSpec::ENEMY_BOARD_OR_HERO;
    if (s == "FRIENDLY_BOARD") return TargetSpec::FRIENDLY_BOARD;
    if (s == "SELF") return TargetSpec::SELF;
    if (s == "ANY") return TargetSpec::ANY;
    return TargetSpec::UNKNOWN;
}
*/
ListeningZone ParseListeningZone(const std::string& str) {
    if (str == "HAND") return ListeningZone::HAND;
    if (str == "DECK") return ListeningZone::DECK;
    if (str == "ANY") return ListeningZone::ANY;
    return ListeningZone::BOARD;
}

CharacterClassType ParseClassLock(const std::string& s) {
    if (s == "SPECIALIST") return CharacterClassType::SPECIALIST;
    if (s == "SPELLWEAVER") return CharacterClassType::SPELLWEAVER;
    if (s == "BLOODBOUND") return CharacterClassType::BLOODBOUND;
    if (s == "SHADE") return CharacterClassType::SHADE;
    if (s == "MACHINIST") return CharacterClassType::MACHINIST;
    if (s == "SOULFORGED") return CharacterClassType::SOULFORGED;
    if (s == "GAMBLER") return CharacterClassType::GAMBLER;
    if (s == "OATHBOUND") return CharacterClassType::OATHBOUND;
    return CharacterClassType::UNKNOWN;
}

AuxiliaryCardType ParseAuxiliaryType(const std::string& s) {
    if (s == "REAGENT") return AuxiliaryCardType::REAGENT;
    if (s == "OATH") return AuxiliaryCardType::OATH;
    if (s == "FRAGMENT") return AuxiliaryCardType::FRAGMENT;
    if (s == "CONSUMABLE") return AuxiliaryCardType::CONSUMABLE;
    if (s == "RELIC") return AuxiliaryCardType::RELIC;
    return AuxiliaryCardType::NONE;
}

std::unordered_map<int, std::shared_ptr<Card>> CardLoader::LoadAll() {
    std::cout << "🔄 Loading Cards from API...\n";

    auto jsonArray = ApiClient::Get("cards/");
    std::unordered_map<int, std::shared_ptr<Card>> result;

    for (const auto& c : jsonArray) {
        if (!c.contains("id") || !c.contains("name") || !c.contains("cost") || !c.contains("rarity") || !c.contains("type")) {
            std::cerr << "❌ Skipping card — missing required fields: " << c.dump() << "\n";
            continue;
        }

        auto card = std::make_shared<Card>();
        card->Name = c["name"];
        card->Cost = c["cost"];

        try {
            card->CardRarity = ParseRarity(c["rarity"]);
            card->Type = ParseCardType(c["type"]);
        } catch (const std::exception& e) {
            std::cerr << "❌ Parse error: " << e.what() << " in card: " << c.dump() << "\n";
            continue;
        }

        card->Text = c.value("text", "");

        if (c.contains("power") && !c["power"].is_null()) {
            card->Power = c["power"];
        } else {
            card->Power = 0; 
        }

        if (c.contains("health") && !c["health"].is_null()) {
            card->Health = c["health"];
        } else {
            card->Health = 0;
        }

        if (c.contains("is_character_card")) {
            card->IsCharacterCard = c["is_character_card"];
        }
        if (c.contains("is_character_exclusive")) {
            card->IsCharacterExclusive = c["is_character_exclusive"];
        }

        if (c.contains("character") && !c["character"].is_null()) {
            int charId = c["character"].get<int>();
            auto character = CharacterRegistry::Instance().Get(charId);
            if (character) {
                card->CharacterRef = character;
            } else {
                std::cerr << "⚠️ Warning: Card '" << card->Name << "' refers to unknown character ID " << charId << "\n";
            }
        }
        
        if (c.contains("subtypes") && c["subtypes"].is_array()) {
            for (const auto& subtypeId : c["subtypes"]) {
                if (!subtypeId.is_number_integer()) continue;
                    auto subtype = SubtypeRegistry::Instance().Get(subtypeId);
                    if (subtype) card->Subtypes.push_back(subtype);
            }
        }

        if (c.contains("classlock") && !c["classlock"].is_null()) {
            card->ClassLock = ParseClassLock(c["classlock"]);
        }

        if (c.contains("auxilarytype") && !c["auxilarytype"].is_null()) {
            card->AuxilaryType = ParseAuxiliaryType(c["auxilarytype"]);
        }
        
        int cardId = c["id"].get<int>();
        result[cardId] = card;

        card->ID = cardId;
        CardRegistry::Instance().Register(cardId, card);
    }
    std::cout << "✅ Loaded " << result.size() << " cards.\n";
    return result;
}
