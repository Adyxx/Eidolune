#include "CardEffectBindingLoader.h"
#include "../Http/ApiClient.h"
#include "../Registry/CardRegistry.h"
#include "../Registry/TriggerRegistry.h"
#include "../Registry/EffectRegistry.h"
#include "../Registry/ConditionRegistry.h"
#include <iostream>

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

std::vector<std::shared_ptr<CardEffectBinding>> CardEffectBindingLoader::LoadAll() {
    std::cout << "🔄 Loading CardEffectBindings from API...\n";

    std::vector<std::shared_ptr<CardEffectBinding>> bindings;
    json jsonData = ApiClient::Get("effectbindings/");

    for (const auto& b : jsonData) {
        try {
            // --- Required IDs ---
            int cardId = b.at("card").get<int>();
            int triggerId = b.at("trigger").get<int>();
            int effectId = b.at("effect").get<int>();

            auto card = CardRegistry::Instance().Get(cardId);
            auto trigger = TriggerRegistry::Instance().GetShared(triggerId);
            auto effect = EffectRegistry::Instance().Get(effectId);

            std::cout << "🔸 Binding Info - cardId: " << cardId
                      << ", triggerId: " << triggerId
                      << ", effectId: " << effectId << "\n";

            if (!card) {
                std::cerr << "❌ Unknown card ID: " << cardId << "\n";
                continue;
            }
            if (!trigger) {
                std::cerr << "❌ Unknown trigger ID: " << triggerId << "\n";
                continue;
            }
            if (!effect) {
                std::cerr << "❌ Unknown effect ID: " << effectId << "\n";
                continue;
            }

            std::cout << "✅ Found card: " << card->Name
                      << ", trigger: " << trigger->ScriptReference
                      << ", effect: " << effect->ScriptReference << "\n";

            // --- Optional Condition ---
            std::shared_ptr<Condition> condition = nullptr;
            if (b.contains("condition") && !b["condition"].is_null()) {
                int conditionId = b["condition"].get<int>();
                condition = ConditionRegistry::Instance().Get(conditionId);
                if (!condition) {
                    std::cerr << "⚠️ Unknown condition ID: " << conditionId << "\n";
                } else {
                    std::cout << "🧩 With condition: " << condition->ScriptReference << "\n";
                }
            }

            // --- Optional Values ---
            // --- Optional Values (now supports static or dynamic) ---
            std::string rawValue;
            if (b.contains("value") && !b["value"].is_null()) {
                rawValue = b["value"].get<std::string>();
            }

            DynamicValueType valueType = StringToDynamicValueType(rawValue);

            std::optional<int> staticValue = std::nullopt;
            if (valueType == DynamicValueType::STATIC_NUMBER && !rawValue.empty()) {
                staticValue = std::stoi(rawValue);
            }


            std::optional<int> conditionValue = std::nullopt;
            if (b.contains("condition_value") && !b["condition_value"].is_null()) {
                conditionValue = b["condition_value"].get<int>();
            }

            // --- Optional Targeting ---
            std::optional<TargetSpec> targeting = std::nullopt;
            if (b.contains("targeting") && !b["targeting"].is_null()) {
                std::string targetStr = b["targeting"].get<std::string>();
                targeting = StringToTargetSpec(targetStr);
            }

            // --- Optional Zone ---
            ListeningZone zone = ListeningZone::ANY;
            if (b.contains("zone") && !b["zone"].is_null()) {
                zone = StringToListeningZone(b["zone"].get<std::string>());
            }

            // --- Optional Scope ---
            TriggerScope scope = TriggerScope::SELF;
            if (b.contains("scope") && !b["scope"].is_null()) {
                scope = StringToTriggerScope(b["scope"].get<std::string>());
            }

            // --- Optional LinkedCard ---
            std::shared_ptr<Card> linkedCard = nullptr;
            if (b.contains("linked_card") && !b["linked_card"].is_null()) {
                int linkedCardId = b["linked_card"].get<int>();
                linkedCard = CardRegistry::Instance().Get(linkedCardId);
                if (!linkedCard) {
                    std::cerr << "⚠️ Unknown linked_card ID: " << linkedCardId << "\n";
                } else {
                    std::cout << "🔗 Linked card: " << linkedCard->Name << "\n";
                }
            }

            // --- Final Binding Creation ---
            auto binding = std::make_shared<CardEffectBinding>(
                card,
                trigger,
                effect,
                condition,
                rawValue,    // full raw string value (could be "3" or "enemy_board_count")
                valueType,   // parsed enum
                staticValue, // only set if it's a number
                targeting
            );


            binding->SetZone(zone);
            binding->SetScope(scope);

            if (conditionValue.has_value()) {
                binding->SetConditionValue(conditionValue.value());
            }

            if (linkedCard) {
                binding->SetLinkedCard(linkedCard);
            }

            card->EffectBindings.push_back(binding);
            bindings.push_back(binding);

            std::cout << "🧠 Attached binding to card '" << card->Name << "' "
                      << "(total bindings now: " << card->EffectBindings.size() << ")\n\n";
        }
        catch (const std::exception& ex) {
            std::cerr << "❌ Failed to parse CardEffectBinding: " << ex.what() << "\n";
        }
    }

    std::cout << "✅ Loaded " << bindings.size() << " CardEffectBindings.\n";
    return bindings;
}
