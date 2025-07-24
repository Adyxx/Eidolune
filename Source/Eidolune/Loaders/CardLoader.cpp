#include "CardLoader.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

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

static TargetSpec ParseTargetSpec(const std::string& s) {
    if (s == "ENEMY_BOARD") return TargetSpec::ENEMY_BOARD;
    if (s == "ENEMY_HERO") return TargetSpec::ENEMY_HERO;
    if (s == "ENEMY_BOARD_OR_HERO") return TargetSpec::ENEMY_BOARD_OR_HERO;
    if (s == "FRIENDLY_BOARD") return TargetSpec::FRIENDLY_BOARD;
    if (s == "SELF") return TargetSpec::SELF;
    if (s == "ANY") return TargetSpec::ANY;
    return TargetSpec::UNKNOWN;
}

std::unordered_map<std::string, std::shared_ptr<Card>> CardLoader::LoadAll(const std::string& filename) {
    std::ifstream file(filename);
    

    if (!file.is_open()) throw std::runtime_error("Could not open card JSON file.");

    json cardArray;
    file >> cardArray;

    std::unordered_map<std::string, std::shared_ptr<Card>> result;

    for (const auto& c : cardArray) {
        auto card = std::make_shared<Card>();
        card->Name = c["name"];
        card->Cost = c["cost"];
        card->CardRarity = ParseRarity(c["rarity"]);
        card->Type = ParseCardType(c["type"]);
        card->Text = c.value("text", "");

        if (c.contains("power")) card->Power = c["power"];
        if (c.contains("health")) card->Health = c["health"];

        for (const auto& e : c["effectBindings"]) {
            auto trigger = std::make_shared<Trigger>(e["trigger"]);
            auto effect = std::make_shared<Effect>(e["effect"]);
            std::optional<int> value = e.contains("value") ? std::make_optional(e["value"].get<int>()) : std::nullopt;
            std::optional<TargetSpec> target = e.contains("targeting") ? std::make_optional(ParseTargetSpec(e["targeting"])) : std::nullopt;

            auto binding = std::make_shared<CardEffectBinding>(card, trigger, effect, nullptr, value, target);
            card->EffectBindings.push_back(binding);
        }

        result[c["id"]] = card;
    }

    return result;
}
