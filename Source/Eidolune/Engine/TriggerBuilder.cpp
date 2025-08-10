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
#include <random>


namespace {
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

        /////////////////////// SILENCE ///////////////////////
        auto* sourceCard = static_cast<GameCard*>(eventCard);

        if (sourceCard && sourceCard->HasStatus(CardStatus::SILENCED)) {
            std::cout << "🚫 " << sourceCard->GetName() << " is silenced — skipping effect trigger.\n";
            return;
        }
        /////////////////////// /////// ///////////////////////

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

        //PrintAllCardsForPlayer(triggerOwner);

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


        std::optional<int> resolvedValue;

        if (binding->GetValue().has_value()) {
            resolvedValue = binding->GetValue();
        } else {
            switch (binding->GetValueType()) {
                case DynamicValueType::YOUR_CARDS_IN_HAND:
                    resolvedValue = triggerOwner ? (int)triggerOwner->Hand.size() : 0;
                    break;
                case DynamicValueType::ENEMY_CARDS_IN_HAND:
                    resolvedValue = triggerOwner && triggerOwner->GetOpponent()
                        ? (int)triggerOwner->GetOpponent()->Hand.size()
                        : 0;
                    break;
                case DynamicValueType::FRIENDLY_BOARD_COUNT:
                    resolvedValue = triggerOwner ? (int)triggerOwner->Board.size() : 0;
                    break;
                case DynamicValueType::ENEMY_BOARD_COUNT:
                    resolvedValue = triggerOwner && triggerOwner->GetOpponent()
                        ? (int)triggerOwner->GetOpponent()->Board.size()
                        : 0;
                    break;
                case DynamicValueType::FRIENDLY_GRAVEYARD_COUNT:
                    resolvedValue = triggerOwner ? (int)triggerOwner->Graveyard.size() : 0;
                    break;
                case DynamicValueType::ENEMY_GRAVEYARD_COUNT:
                    resolvedValue = triggerOwner && triggerOwner->GetOpponent()
                        ? (int)triggerOwner->GetOpponent()->Graveyard.size()
                        : 0;
                    break;
                case DynamicValueType::TURN_NUMBER:
                    resolvedValue = triggerOwner ? triggerOwner->TurnCount : 0;
                    break;
                case DynamicValueType::OWNER_HEALTH:
                    resolvedValue = triggerOwner ? triggerOwner->Health : 0;
                    break;
                case DynamicValueType::OPPONENT_HEALTH:
                    resolvedValue = triggerOwner && triggerOwner->GetOpponent()
                        ? triggerOwner->GetOpponent()->Health
                        : 0;
                    break;
                default:
                    resolvedValue = std::nullopt;
                    break;
            }
        }

        /////////////////////////////////////

        TargetSpec targetingSpec = binding->GetTargetSpec().value_or(TargetSpec::SELF);
        TargetingRule targetingRule = binding->targetingRule;
        bool targetRequired = effect->RequiresTarget;

        std::vector<Target> possibleTargets;

        if (triggerOwner) {
            possibleTargets = GameActions::GetTargets(triggerOwner, triggerOwner->GetOpponent(), targetingSpec);
        }

        if (targetingRule == TargetingRule::MANUAL) {
            possibleTargets.erase(
                std::remove_if(possibleTargets.begin(), possibleTargets.end(),
                    [&](const Target& t) {
                        auto* gc = static_cast<GameCard*>(t.ptr);
                        return gc && !gc->CanBeTargetedBy(eventCard);
                    }),
                possibleTargets.end()
            );
        }

        Target resolvedTarget = { Target::Type::NONE, nullptr };

        // If no possible targets
        if (possibleTargets.empty()) {
            if (!targetRequired && triggerOwner) {
                resolvedTarget = Target::FromPlayer(triggerOwner);
            } else if (targetRequired) {
                std::cout << "❌ Effect requires a target but none found.\n";
                return;
            }
        }
        // If AOE, apply effect to all targets immediately
        else if (targetingRule == TargetingRule::AOE) {
            for (auto& t : possibleTargets) {
                effectFunc(eventCard, t, resolvedValue);
            }
            return;
        }
        // If RANDOM
        else if (targetingRule == TargetingRule::RANDOM) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, static_cast<int>(possibleTargets.size()) - 1);
            resolvedTarget = possibleTargets[dist(gen)];
        }
        // If WEAKEST (lowest HP, tie-breaker: lowest attack)
        else if (targetingRule == TargetingRule::WEAKEST) {
            resolvedTarget = *std::min_element(possibleTargets.begin(), possibleTargets.end(),
                [](const Target& a, const Target& b) {
                    auto* ca = static_cast<GameCard*>(a.ptr);
                    auto* cb = static_cast<GameCard*>(b.ptr);
                    if (!ca || !cb) return false;
                    if (ca->CurrentHealth() != cb->CurrentHealth())
                        return ca->CurrentHealth() < cb->CurrentHealth();
                    return ca->CurrentAttack() < cb->CurrentAttack();
                });
        }
        // If STRONGEST (highest attack, tie-breaker: highest HP)
        else if (targetingRule == TargetingRule::STRONGEST) {
            resolvedTarget = *std::max_element(possibleTargets.begin(), possibleTargets.end(),
                [](const Target& a, const Target& b) {
                    auto* ca = static_cast<GameCard*>(a.ptr);
                    auto* cb = static_cast<GameCard*>(b.ptr);
                    if (!ca || !cb) return false;
                    if (ca->CurrentAttack() != cb->CurrentAttack())
                        return ca->CurrentAttack() > cb->CurrentAttack();
                    return ca->CurrentHealth() > cb->CurrentHealth();
                });
        }
        // Default MANUAL or fallback
        else {
            resolvedTarget = GameActions::ChooseTarget(triggerOwner, possibleTargets);
            if (!resolvedTarget.ptr) {
                std::cout << "❌ No target selected.\n";
                return;
            }
        }

        /////////////////////////////////////

        std::cout << "✨ Trigger fired: " << binding->GetTrigger()->ScriptReference << "\n";
        
        auto& ctxC = GetEffectCallContext();
        ctxC.CurrentBinding = binding;
        effectFunc(eventCard, resolvedTarget, resolvedValue);
        ctxC.CurrentBinding = nullptr;

    };
}
