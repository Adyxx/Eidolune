

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

        //std::cout << "🧪 Trigger fired: " << binding->GetTrigger()->ScriptReference << "\n";

        if (!eventCardPtr || !triggerCard) {
            std::cout << "❌ Missing card references in context\n";
            return;
        }

        auto* eventCard = eventCardPtr.get();
        //std::cout << "🔗 Binding ptr: " << binding.get() << "\n";

        switch (binding->GetScope()) {
            case TriggerScope::SELF:
                //std::cout << "SELF\n";
                //std::cout << "  eventCard:  ID = " << eventCard->Id << ", Name = " << eventCard->Model->Name << "\n";
                //std::cout << "  triggerCard: ID = " << triggerCard->Id << ", Name = " << triggerCard->Model->Name << "\n";
                
                if (eventCard->Id != triggerCard->Id) {
                    // std::cout << "  ❌ Skipping: ID mismatch\n";
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

        if (binding->HasZone()) {
            if (eventCard->Zone != binding->GetZoneAsCardZone()) return;
        }

        auto effect = binding->GetEffect();
        auto effectFunc = effect->GetExecutable();
        if (!effectFunc) {
            std::cout << "❌ Effect function is null.\n";
            return;
        }
        ////////////

        //void* resolvedTarget = nullptr;
        //Target resolvedTarget = Target{Target::Type::NONE, nullptr};

        TargetSpec targetingSpec = binding->GetTargetSpec().value_or(TargetSpec::SELF);

        bool targetRequired = effect->RequiresTarget;

        std::vector<Target> possibleTargets = GameActions::GetTargets(triggerOwner, triggerOwner->GetOpponent(), targetingSpec);
/*
        if (possibleTargets.empty()) {
            std::cout << "AAA\n"; 
            if (!targetRequired) {
                resolvedTarget = triggerOwner;

            } else {
                std::cout << "❌ Effect '" << effect->Name << "' requires a target, but none found.\n";
                return;
            }
        } else if (possibleTargets.size() == 1) {
            std::cout << "BBB\n"; 
            resolvedTarget = possibleTargets.front().ptr;
        } else {
            std::cout << "CCC\n"; 
            Target chosen = GameActions::ChooseTarget(triggerOwner, possibleTargets);
            if (chosen.ptr == nullptr) {
                std::cout << "❌ No target selected.\n";
                return;
            }
            resolvedTarget = chosen.ptr;;
        }

*/


        Target resolvedTarget = { Target::Type::NONE, nullptr };

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






        ////////////

        // print resolvedtarget
        /*
        if (resolvedTarget) {
            if (auto* card = static_cast<GameCard*>(resolvedTarget)) {
                std::cout << "0🔗 Resolved target: " << card->GetName() << " (ID: " << card->Id << ")\n";
            } else if (auto* player = static_cast<Player*>(resolvedTarget)) {
                std::cout << "1🔗 Resolved target: Player " << player->GetName() << "\n";
            } else {
                std::cout << "2🔗 Resolved target: Unknown type\n";
            }
        } else {
            std::cout << "3🔗 Resolved target: nullptr\n";
        }
        */


        std::cout << "✨ Trigger fired: " << binding->GetTrigger()->ScriptReference << "\n";
        //std::cout << "   → Trigger: " << triggerCard->ToString() << "\n";
        //std::cout << "   → EventCard: " << eventCard->ToString() << "\n";

        
        //effectFunc(eventCard, resolvedTarget, binding->GetValue());
        effectFunc(eventCard, resolvedTarget, binding->GetValue());


    };
}

