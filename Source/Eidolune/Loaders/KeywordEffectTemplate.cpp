#include "KeywordEffectTemplateLoader.h"
#include "../Http/ApiClient.h"
#include "TriggerRegistry.h"
#include "EffectRegistry.h"
#include "ConditionRegistry.h"
#include "CardRegistry.h"
#include <iostream>
#include "../Registry/KeywordTemplateRegistry.h"

inline ListeningZone StringToListeningZone(const std::string& str) {
    if (str == "HAND") return ListeningZone::HAND;
    if (str == "DECK") return ListeningZone::DECK;
    if (str == "BOARD") return ListeningZone::BOARD;
    if (str == "GRAVEYARD") return ListeningZone::GRAVEYARD;
    if (str == "OATHZONE") return ListeningZone::OATHZONE;
    return ListeningZone::ANY;
}

inline TriggerScope StringToTriggerScope(const std::string& str) {
    if (str == "OTHER_FRIENDLY") return TriggerScope::OTHER_FRIENDLY;
    if (str == "OTHER_ENEMY") return TriggerScope::OTHER_ENEMY;
    if (str == "ANY_FRIENDLY") return TriggerScope::ANY_FRIENDLY;
    if (str == "ANY_OTHER") return TriggerScope::ANY_OTHER;
    if (str == "ANY") return TriggerScope::ANY;
    if (str == "GLOBAL") return TriggerScope::GLOBAL;
    return TriggerScope::SELF;
}

inline TargetSpec StringToTargetSpec(const std::string& str) {
    if (str == "ENEMY_BOARD_OR_HERO") return TargetSpec::ENEMY_BOARD_OR_HERO;
    if (str == "ENEMY_HERO") return TargetSpec::ENEMY_HERO;
    if (str == "ENEMY_BOARD") return TargetSpec::ENEMY_BOARD;
    if (str == "RANDOM_ENEMY") return TargetSpec::RANDOM_ENEMY;
    if (str == "RANDOM_ALLY") return TargetSpec::RANDOM_ALLY;
    if (str == "FRIENDLY_BOARD") return TargetSpec::FRIENDLY_BOARD;
    if (str == "FRIENDLY_HERO") return TargetSpec::FRIENDLY_HERO;
    if (str == "SELF") return TargetSpec::SELF;
    if (str == "ANY") return TargetSpec::ANY;
    return TargetSpec::UNKNOWN;
}


inline DynamicValueType StringToDynamicValueType(const std::string& str) {
    if (str == "your_cards_in_hand") return DynamicValueType::YOUR_CARDS_IN_HAND;
    if (str == "enemy_cards_in_hand") return DynamicValueType::ENEMY_CARDS_IN_HAND;
    if (str == "friendly_board_count") return DynamicValueType::FRIENDLY_BOARD_COUNT;
    if (str == "enemy_board_count") return DynamicValueType::ENEMY_BOARD_COUNT;
    if (str == "friendly_graveyard_count") return DynamicValueType::FRIENDLY_GRAVEYARD_COUNT;
    if (str == "enemy_graveyard_count") return DynamicValueType::ENEMY_GRAVEYARD_COUNT;
    if (str == "turn_number") return DynamicValueType::TURN_NUMBER;
    if (str == "owner_health") return DynamicValueType::OWNER_HEALTH;
    if (str == "opponent_health") return DynamicValueType::OPPONENT_HEALTH;
    if (str.find_first_not_of("0123456789") == std::string::npos) return DynamicValueType::STATIC_NUMBER;
    return DynamicValueType::UNKNOWN;
}

inline TargetingRule StringToTargetingRule(const std::string& str) {
    if (str == "MANUAL") return TargetingRule::MANUAL;
    if (str == "AOE") return TargetingRule::AOE;
    if (str == "RANDOM") return TargetingRule::RANDOM;
    if (str == "WEAKEST") return TargetingRule::WEAKEST;
    if (str == "STRONGEST") return TargetingRule::STRONGEST;
    return TargetingRule::NONE;
}


std::vector<std::shared_ptr<KeywordEffectTemplate>> KeywordEffectTemplateLoader::LoadAll() {
    std::cout << "🔄 Loading KeywordEffectTemplates from API...\n";

    std::vector<std::shared_ptr<KeywordEffectTemplate>> templates;
    json jsonData = ApiClient::Get("keywordtemplates/");

    for (const auto& item : jsonData) {
        try {
            int id = item.at("id").get<int>();
            std::string name = item.at("name").get<std::string>();

            // Lookup trigger & effect
            int triggerId = item.at("trigger").get<int>();
            auto trigger = TriggerRegistry::Instance().GetShared(triggerId);
            if (!trigger) {
                std::cerr << "❌ Unknown trigger ID: " << triggerId << " for keyword template " << id << "\n";
                continue;
            }

            int effectId = item.at("effect").get<int>();
            auto effect = EffectRegistry::Instance().Get(effectId);
            if (!effect) {
                std::cerr << "❌ Unknown effect ID: " << effectId << " for keyword template " << id << "\n";
                continue;
            }

            // Handle raw value & dynamic/static type
            std::string rawValue;
            if (item.contains("value") && !item["value"].is_null()) {
                rawValue = item["value"].get<std::string>();
            }
            DynamicValueType valueType = StringToDynamicValueType(rawValue);

            std::optional<int> staticValue = std::nullopt;
            if (valueType == DynamicValueType::STATIC_NUMBER && !rawValue.empty()) {
                try {
                    staticValue = std::stoi(rawValue);
                } catch (...) {
                    std::cerr << "⚠️ Invalid static number value '" << rawValue << "' for keyword template " << id << "\n";
                }
            }

            // Condition
            std::shared_ptr<Condition> condition = nullptr;
            if (item.contains("condition") && !item["condition"].is_null()) {
                int conditionId = item["condition"].get<int>();
                condition = ConditionRegistry::Instance().Get(conditionId);
                if (!condition) {
                    std::cerr << "⚠️ Unknown condition ID: " << conditionId << " for keyword template " << id << "\n";
                }
            }

            std::optional<int> conditionValue = std::nullopt;
            if (item.contains("condition_value") && !item["condition_value"].is_null()) {
                conditionValue = item["condition_value"].get<int>();
            }

            // Targeting info
            TargetSpec targeting = TargetSpec::UNKNOWN;
            if (item.contains("targeting") && !item["targeting"].is_null()) {
                targeting = StringToTargetSpec(item["targeting"].get<std::string>());
            }

            TargetingRule targetingRule = TargetingRule::NONE;
            if (item.contains("targeting_rule") && !item["targeting_rule"].is_null()) {
                targetingRule = StringToTargetingRule(item["targeting_rule"].get<std::string>());
            }

            ListeningZone zone = ListeningZone::ANY;
            if (item.contains("zone") && !item["zone"].is_null()) {
                zone = StringToListeningZone(item["zone"].get<std::string>());
            }

            TriggerScope scope = TriggerScope::SELF;
            if (item.contains("scope") && !item["scope"].is_null()) {
                scope = StringToTriggerScope(item["scope"].get<std::string>());
            }

            // Linked card
            std::shared_ptr<Card> linkedCard = nullptr;
            if (item.contains("linked_card") && !item["linked_card"].is_null()) {
                int linkedCardId = item["linked_card"].get<int>();
                linkedCard = CardRegistry::Instance().Get(linkedCardId);
                if (!linkedCard) {
                    std::cerr << "⚠️ Unknown linked_card ID: " << linkedCardId << " for keyword template " << id << "\n";
                }
            }

            // Create template
            auto keywordTemplate = std::make_shared<KeywordEffectTemplate>(
                id, name,
                trigger, effect,
                rawValue, valueType,
                condition, conditionValue,
                targeting, targetingRule,
                zone, scope,
                linkedCard
            );

            // Store static value directly if present
            if (staticValue.has_value()) {
                // Assuming KeywordEffectTemplate has a member to hold it
                keywordTemplate->ConditionValue = staticValue;
            }

            // Push to vector
            templates.push_back(keywordTemplate);

            // Register globally
            KeywordEffectTemplateRegistry::Instance().Register(keywordTemplate);
        }
        catch (const std::exception& ex) {
            std::cerr << "❌ Failed to parse KeywordEffectTemplate: " << ex.what() << "\n";
        }
    }

    std::cout << "✅ Loaded " << templates.size() << " KeywordEffectTemplates.\n";
    return templates;
}