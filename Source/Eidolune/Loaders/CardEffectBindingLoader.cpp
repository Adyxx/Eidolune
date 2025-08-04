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

            // --- Optional Value ---
            std::optional<int> value = std::nullopt;
            if (b.contains("value") && !b["value"].is_null()) {
                value = b["value"].get<int>();
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

            // --- Final Binding Creation ---
            auto binding = std::make_shared<CardEffectBinding>(
                card,
                trigger,
                effect,
                condition,
                value,
                targeting
            );

            binding->SetZone(zone);
            binding->SetScope(scope);

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
