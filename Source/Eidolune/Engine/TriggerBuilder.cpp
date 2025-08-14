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
#include "../Registry/KeywordTemplateRegistry.h"


bool TriggerBuilder::IsTriggerScopeMatch(GameCard* eventCard, GameCard* triggerCard, TriggerScope scope) {

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
            case TriggerScope::EQUIPPED_UNIT: {

                if (!triggerCard || !eventCard) return false;
                auto attached = eventCard->AttachedTo.lock();
                
                // return true if either card is attached to the other
                return (attached && attached.get() == triggerCard) || 
                    (triggerCard->AttachedTo.lock().get() == eventCard);
            }
            case TriggerScope::ANY:
                return true;
            default:
                std::cout << "❌ Unhandled TriggerScope enum.\n";
                return false;
        }
}

bool TriggerBuilder::IsSilenced(GameCard* card) {
    if (card && card->HasStatus(CardStatus::SILENCED)) {
        std::cout << "🚫 " << card->GetName() << " is silenced — skipping effect trigger.\n";
        return true;
    }
    return false;
}

std::optional<int> TriggerBuilder::ResolveValue(std::shared_ptr<CardEffectBinding> binding, Player* triggerOwner) {
    if (binding->GetValue().has_value()) return binding->GetValue();

    switch (binding->GetValueType()) {
        case DynamicValueType::YOUR_CARDS_IN_HAND:
            return triggerOwner ? (int)triggerOwner->Hand.size() : 0;
        case DynamicValueType::ENEMY_CARDS_IN_HAND:
            return triggerOwner && triggerOwner->GetOpponent()
                ? (int)triggerOwner->GetOpponent()->Hand.size()
                : 0;
        case DynamicValueType::FRIENDLY_BOARD_COUNT:
            return triggerOwner ? (int)triggerOwner->Board.size() : 0;
        case DynamicValueType::ENEMY_BOARD_COUNT:
            return triggerOwner && triggerOwner->GetOpponent()
                ? (int)triggerOwner->GetOpponent()->Board.size()
                : 0;
        case DynamicValueType::FRIENDLY_GRAVEYARD_COUNT:
            return triggerOwner ? (int)triggerOwner->Graveyard.size() : 0;
        case DynamicValueType::ENEMY_GRAVEYARD_COUNT:
            return triggerOwner && triggerOwner->GetOpponent()
                ? (int)triggerOwner->GetOpponent()->Graveyard.size()
                : 0;
        case DynamicValueType::TURN_NUMBER:
            return triggerOwner ? triggerOwner->TurnCount : 0;
        case DynamicValueType::OWNER_HEALTH:
            return triggerOwner ? triggerOwner->Health : 0;
        case DynamicValueType::OPPONENT_HEALTH:
            return triggerOwner && triggerOwner->GetOpponent()
                ? triggerOwner->GetOpponent()->Health
                : 0;
        default: {
            // Keyword lookup fallback
            const std::string& raw = binding->GetRawValue();
            auto keywordTemplate = KeywordEffectTemplateRegistry::Instance().GetByName(raw);
            if (keywordTemplate) {
                return keywordTemplate->Id; // pass template ID to effect
            }
            return std::nullopt;
        }
    }
}

Target TriggerBuilder::ResolveTarget(std::shared_ptr<CardEffectBinding> binding,Player* triggerOwner,
    GameCard* eventCard,
    std::function<void(void*, Target, std::optional<int>)> effectFunc,
    std::optional<int> resolvedValue) {

    TargetSpec targetingSpec = binding->GetTargetSpec().value_or(TargetSpec::SELF);
    TargetingRule targetingRule = binding->targetingRule;
    bool targetRequired = binding->GetEffect()->RequiresTarget;

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

    if (possibleTargets.empty()) {
        if (!targetRequired && triggerOwner) {
            return Target::FromPlayer(triggerOwner);
        } else if (targetRequired) {
            std::cout << "❌ Effect requires a target but none found.\n";
            return { Target::Type::NONE, nullptr };
        }
    }
    else if (targetingRule == TargetingRule::AOE) {
        for (auto& t : possibleTargets) {
            effectFunc(eventCard, t, resolvedValue);
        }
        return { Target::Type::NONE, nullptr }; 
    }
    else if (targetingRule == TargetingRule::RANDOM) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, static_cast<int>(possibleTargets.size()) - 1);
        return possibleTargets[dist(gen)];
    }
    else if (targetingRule == TargetingRule::WEAKEST) {
        return *std::min_element(possibleTargets.begin(), possibleTargets.end(),
            [](const Target& a, const Target& b) {
                auto* ca = static_cast<GameCard*>(a.ptr);
                auto* cb = static_cast<GameCard*>(b.ptr);
                if (!ca || !cb) return false;
                if (ca->CurrentHealth() != cb->CurrentHealth())
                    return ca->CurrentHealth() < cb->CurrentHealth();
                return ca->CurrentAttack() < cb->CurrentAttack();
            });
    }
    else if (targetingRule == TargetingRule::STRONGEST) {
        return *std::max_element(possibleTargets.begin(), possibleTargets.end(),
            [](const Target& a, const Target& b) {
                auto* ca = static_cast<GameCard*>(a.ptr);
                auto* cb = static_cast<GameCard*>(b.ptr);
                if (!ca || !cb) return false;
                if (ca->CurrentAttack() != cb->CurrentAttack())
                    return ca->CurrentAttack() > cb->CurrentAttack();
                return ca->CurrentHealth() > cb->CurrentHealth();
            });
    }
    else {
        return GameActions::ChooseTarget(triggerOwner, possibleTargets);
    }

    return resolvedTarget;
}


std::function<void(std::unordered_map<std::string, void*>)>
TriggerBuilder::Build(std::shared_ptr<CardEffectBinding> binding) {
    return [binding](std::unordered_map<std::string, void*> ctx) {
        if (!binding) return;

        auto eventCardPtr = binding->GetEventGameCard();
        auto* eventCard = eventCardPtr ? eventCardPtr.get() : nullptr;

        if (IsSilenced(eventCard)) return;

        auto* triggerCard = ctx.count("source") ? static_cast<GameCard*>(ctx["source"]) : nullptr;
        auto* triggerOwner = ctx.count("owner") ? static_cast<Player*>(ctx["owner"]) : nullptr;


        
        if (!IsTriggerScopeMatch(eventCard, triggerCard, binding->GetScope())) return;

        if (binding->HasZone() && eventCard->Zone != binding->GetZoneAsCardZone()) return;

        auto effect = binding->GetEffect();
        if (!effect || !effect->GetExecutable()) return;

        auto resolvedValue = ResolveValue(binding, triggerOwner);
        auto resolvedTarget = ResolveTarget(binding, triggerOwner, eventCard, effect->GetExecutable(), resolvedValue);

        if (!resolvedTarget.ptr && effect->RequiresTarget) return;

        std::cout << "✨ Trigger fired: " << binding->GetTrigger()->ScriptReference << "\n";
        auto& ctxC = GetEffectCallContext();
        ctxC.CurrentBinding = binding;
        effect->GetExecutable()(eventCard, resolvedTarget, resolvedValue);
        ctxC.CurrentBinding = nullptr;
    };
}
