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

        if (c.contains("power")) card->Power = c["power"];
        if (c.contains("health")) card->Health = c["health"];
        /*
        if (c.contains("effectBindings") && c["effectBindings"].is_array()) {
            for (const auto& e : c["effectBindings"]) {
                if (!e.contains("trigger") || !e.contains("effect")) {
                    std::cerr << "⚠️ Skipping effectBinding — missing trigger/effect: " << e.dump() << "\n";
                    continue;
                }

                auto trigger = std::make_shared<Trigger>(e["trigger"]);
                auto effect = std::make_shared<Effect>(e["effect"]);

                std::optional<int> value = e.contains("value") ? std::make_optional(e["value"].get<int>()) : std::nullopt;
                std::optional<TargetSpec> target = e.contains("targeting") ? std::make_optional(ParseTargetSpec(e["targeting"])) : std::nullopt;

                std::shared_ptr<Condition> condition = nullptr;
                if (e.contains("condition") && e["condition"].is_object()) {
                    const auto& condJson = e["condition"];
                    condition = std::make_shared<Condition>(
                        condJson.value("name", "unnamed_condition"),
                        condJson.value("ref", ""),
                        condJson.value("description", "")
                    );
                }

                auto binding = std::make_shared<CardEffectBinding>(card, trigger, effect, condition, value, target);
                std::string zoneStr = e.value("zone", "ANY");
                binding->SetZone(ParseListeningZone(zoneStr));

                card->EffectBindings.push_back(binding);
            }
        }
        */

        
        int cardId = c["id"].get<int>();
        result[cardId] = card;
        CardRegistry::Instance().Register(cardId, card);
    }
    std::cout << "✅ Loaded " << result.size() << " cards.\n";
    return result;
}
