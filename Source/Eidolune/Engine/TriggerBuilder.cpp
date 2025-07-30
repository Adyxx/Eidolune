

#include "TriggerBuilder.h"
#include "CardEffectBinding.h"
#include "Effect.h"
#include "GameCard.h"
#include "Trigger.h"
#include <iostream>
#include "Types.h"
#include "GameActions.h"
#include "Target.h"

std::function<void(std::unordered_map<std::string, void*>)>
TriggerBuilder::Build(std::shared_ptr<CardEffectBinding> binding) {
    return [binding](std::unordered_map<std::string, void*> ctx) {
        auto eventCardPtr = binding->GetEventGameCard();
        auto* triggerCard = static_cast<GameCard*>(ctx.at("source"));
        auto* triggerOwner = static_cast<Player*>(ctx.at("owner"));

        std::cout << "TriggerBuilder ENTERED\n";
        if (!eventCardPtr || !triggerCard) {
            std::cout << "❌ Missing card references in context\n";
            return;
        }
        std::cout << "AAAAAA\n";
        auto* eventCard = eventCardPtr.get();

        switch (binding->GetScope()) {
            case TriggerScope::SELF:
                if (eventCard->Id != triggerCard->Id) {
                    return;
                }
                break;

            case TriggerScope::OTHER_FRIENDLY:
                if (eventCard == triggerCard) return;
                if (!eventCard->Owner || !triggerCard->Owner) return;
                if (eventCard->Owner != triggerCard->Owner) return;
                break;

            case TriggerScope::OTHER_ENEMY:
                if (eventCard == triggerCard) return;
                if (!eventCard->Owner || !triggerCard->Owner) return;
                if (eventCard->Owner == triggerCard->Owner) return;
                break;

            case TriggerScope::ANY_OTHER:
                if (eventCard == triggerCard) return;
                break;

            case TriggerScope::ANY:
                break;
            default:
                std::cout << "❌ Unhandled TriggerScope: " << "\n";
        }
        std::cout << "BBBBBB\n";

        if (binding->HasZone()) {
            if (eventCard->Zone != binding->GetZoneAsCardZone()) return;
        }
        std::cout << "CCCCCC\n";
        auto effect = binding->GetEffect();
        auto effectFunc = effect->GetExecutable();
        if (!effectFunc) {
            std::cout << "❌ Effect function is null.\n";
            return;
        }
        std::cout << "DDDDDDD\n";
        TargetSpec targetingSpec = binding->GetTargetSpec().value_or(TargetSpec::SELF);

        bool targetRequired = effect->RequiresTarget;

        std::vector<Target> possibleTargets = GameActions::GetTargets(triggerOwner, triggerOwner->GetOpponent(), targetingSpec);

        Target resolvedTarget = { Target::Type::NONE, nullptr };
        std::cout << "EEEEEEE\n";
        if (possibleTargets.empty()) {
            if (!targetRequired) {
                resolvedTarget = Target::FromPlayer(triggerOwner);
            } else {
                std::cout << "❌ Effect requires target but none found.\n";
                return;
            }
        } else if (possibleTargets.size() == 1) {
            resolvedTarget = possibleTargets.front();
        } else {
            Target chosen = GameActions::ChooseTarget(triggerOwner, possibleTargets);
            if (!chosen.ptr) {
                std::cout << "❌ No target selected.\n";
                return;
            }
            resolvedTarget = chosen;
        }
        std::cout << "FFFFFFF\n";
        std::cout << "✨ Trigger fired: " << binding->GetTrigger()->ScriptReference << "\n";

        effectFunc(eventCard, resolvedTarget, binding->GetValue());
    };
}

