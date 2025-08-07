#include "TriggerBuilder.h"
#include "CardEffectBinding.h"
#include "Effect.h"
#include "GameCard.h"
#include "Trigger.h"
#include <iostream>
#include "Types.h"
#include "GameActions.h"
#include "Target.h"
#include "EffectContext.h"




namespace {
void PrintAllCardsForPlayer(Player* player) {
    auto printZone = [](const std::string& name, const std::vector<std::shared_ptr<GameCard>>& zone) {
        std::cout << "📦 " << name << " [" << zone.size() << "]: ";
        for (const auto& card : zone) {
            std::cout << card->GetName() << " ";
        }
        std::cout << "\n";
    };

    if (!player) {
        std::cout << "⚠️ Cannot print cards: player is null.\n";
        return;
    }

    std::cout << "\n🔍 ZONE STATE FOR PLAYER " << player->PlayerIndex << ":\n";
    printZone("Hand", player->Hand);
    printZone("Board", player->Board);
    printZone("Graveyard", player->Graveyard);
    printZone("DrawPile", player->DrawPile);
    printZone("Exile", player->ExileZone);
    printZone("OathZone", player->OathZone);
    std::cout << "--------------------------------\n";
}
    // Helper to determine if the trigger matches the scope rules
bool IsTriggerScopeMatch(GameCard* eventCard, GameCard* triggerCard, TriggerScope scope) {
    switch (scope) {
        case TriggerScope::GLOBAL:
            return true;
        case TriggerScope::SELF:
            return triggerCard && eventCard && eventCard->Id == triggerCard->Id;
        case TriggerScope::OTHER_FRIENDLY:
            return triggerCard && eventCard && eventCard != triggerCard &&
                   eventCard->Owner && triggerCard->Owner &&
                   eventCard->Owner == triggerCard->Owner;
        case TriggerScope::ANY_FRIENDLY:
            return triggerCard && eventCard &&
                   eventCard->Owner && triggerCard->Owner &&
                   eventCard->Owner == triggerCard->Owner;
        case TriggerScope::OTHER_ENEMY:
            return triggerCard && eventCard && eventCard != triggerCard &&
                   eventCard->Owner && triggerCard->Owner &&
                   eventCard->Owner != triggerCard->Owner;
        case TriggerScope::ANY_OTHER:
            return triggerCard && eventCard && eventCard != triggerCard;
        case TriggerScope::ANY:
            return true;
        default:
            std::cout << "❌ Unhandled TriggerScope enum.\n";
            return false;
    }
}




}

std::function<void(std::unordered_map<std::string, void*>)>
TriggerBuilder::Build(std::shared_ptr<CardEffectBinding> binding) {
    return [binding](std::unordered_map<std::string, void*> ctx) {
        //std::cout << "🚨 Trigger callback called for event: " << binding->GetTrigger()->Event << "\n";

        if (!binding) {
            std::cout << "❌ Binding is null.\n";
            return;
        }

        auto eventCardPtr = binding->GetEventGameCard();
        auto* eventCard = eventCardPtr ? eventCardPtr.get() : nullptr;

        auto* triggerCard = ctx.count("source") ? static_cast<GameCard*>(ctx["source"]) : nullptr;
        auto* triggerOwner = ctx.count("owner") ? static_cast<Player*>(ctx["owner"]) : nullptr;

        if (!eventCard) {
            std::cout << "❌ eventCard is null.\n";
            return;
        }

        bool requiresTriggerCard = binding->GetScope() != TriggerScope::GLOBAL;
        if (requiresTriggerCard && !triggerCard) {
            std::cout << "❌ triggerCard is required but missing.\n";
            return;
        }

        if (requiresTriggerCard && !triggerOwner) {
            std::cout << "❌ triggerOwner is required but missing.\n";
            return;
        }

        if (binding->GetScope() == TriggerScope::GLOBAL) {
            auto* turnPlayer = ctx.count("turn_player") ? static_cast<Player*>(ctx["turn_player"]) : nullptr;
            if (!eventCard->Owner || !turnPlayer) {
                std::cout << "❌ GLOBAL trigger requires eventCard->Owner and turn_player.\n";
                return;
            }
            if (eventCard->Owner != turnPlayer) {
                // It's not this card's owner's turn, so skip
                return;
            }
            if (eventCard->Owner->PlayerIndex != turnPlayer->PlayerIndex) return;
        } else {
            if (!IsTriggerScopeMatch(eventCard, triggerCard, binding->GetScope())) {
                return;
            }
        }

        if (binding->HasZone() && eventCard->Zone != binding->GetZoneAsCardZone()) {
            return;
        }

        PrintAllCardsForPlayer(triggerOwner);

        std::cout << "📌 Trigger is using Effect pointer: " << binding->GetEffect() << " (" << binding->BoundEffect->ScriptReference << ")\n";

        auto effect = binding->GetEffect();
        if (!effect) {
            std::cout << "❌ Effect is null.\n";
            return;
        }

        auto effectFunc = effect->GetExecutable();
        if (!effectFunc) {
            std::cout << "❌ Effect function is null.\n";
            return;
        }

        TargetSpec targetingSpec = binding->GetTargetSpec().value_or(TargetSpec::SELF);
        bool targetRequired = effect->RequiresTarget;

        std::vector<Target> possibleTargets;

        if (triggerOwner) {
            possibleTargets = GameActions::GetTargets(triggerOwner, triggerOwner->GetOpponent(), targetingSpec);
        }

        Target resolvedTarget = { Target::Type::NONE, nullptr };

        if (possibleTargets.empty()) {
            if (!targetRequired && triggerOwner) {
                resolvedTarget = Target::FromPlayer(triggerOwner);
            } else if (targetRequired) {
                std::cout << "❌ Effect requires a target but none found.\n";
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

        std::cout << "✨ Trigger fired: " << binding->GetTrigger()->ScriptReference << "\n";
        
        std::cout << "[FROM TriggerBuilder]: " << binding->ToString() <<"\n";
        //effectFunc(eventCard, resolvedTarget, binding->GetValue());

        // Inject binding into context
        auto& ctxC = GetEffectCallContext();
        ctxC.CurrentBinding = binding;

        // Call the actual effect
        effectFunc(eventCard, resolvedTarget, binding->GetValue());

        // Clear context after execution to avoid leaks between triggers
        ctxC.CurrentBinding = nullptr;

    };
}

