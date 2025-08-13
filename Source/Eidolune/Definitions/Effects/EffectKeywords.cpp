

// granting/removing keyword, and keyword logic

#include "Target.h"
#include <optional>
#include "KeywordTemplateRegistry.h"
#include "CardEffectBinding.h"
#include "CardUtils.h"

void GrantKeyword(void* source, Target target, std::optional<int> value) {
    if (!value.has_value()) return;

    int keywordId = value.value();
    auto keywordTemplate = KeywordEffectTemplateRegistry::Instance().Get(keywordId);
    if (!keywordTemplate) {
        std::cerr << "❌ GrantKeyword: Unknown keyword template ID " << keywordId << "\n";
        return;
    }

    auto* targetCard = static_cast<GameCard*>(target.ptr);
    if (!targetCard || !targetCard->Owner || !targetCard->Owner->Observer) return;

    auto runtimeBinding = std::make_shared<CardEffectBinding>(
        nullptr,
        keywordTemplate->TriggerPtr,
        keywordTemplate->EffectPtr,
        keywordTemplate->ConditionPtr,
        keywordTemplate->RawValue,
        keywordTemplate->ValueType,
        keywordTemplate->StaticValue,
        keywordTemplate->Targeting
    );

    runtimeBinding->SetZone(keywordTemplate->Zone);
    runtimeBinding->SetScope(keywordTemplate->Scope);
    runtimeBinding->SetTargetingRule(keywordTemplate->TargetingRuleValue);
    runtimeBinding->SetLinkedCard(keywordTemplate->LinkedCard);

    if (keywordTemplate->ConditionValue.has_value()) {
        runtimeBinding->SetConditionValue(keywordTemplate->ConditionValue.value());
    }

    //targetCard->Model->EffectBindings.push_back(runtimeBinding);
    targetCard->RuntimeEffectBindings.push_back(runtimeBinding);

    CardUtils::SubscribeCardTriggers(targetCard->shared_from_this(), targetCard->Owner->Observer);

    std::cout << "✅ Granted keyword [" << keywordTemplate->Name << "] to " << targetCard->GetName() << "\n";
}

void ApplyHaste(void* source, Target  target, std::optional<int> value) {
    auto* card = static_cast<GameCard*>(source);
    card->SummoningSickness = false;
    std::cout << "[Haste] triggered.\n";
}

void ApplySilence(void* source, Target target, std::optional<int>) {
    auto* card = static_cast<GameCard*>(target.ptr);
    if (!card) return;
    card->AddStatus(CardStatus::SILENCED);
    std::cout << " " << card->GetName() << " is silenced.\n";
}


void ApplyHexproof(void* source, Target target, std::optional<int>) {
    auto* card = static_cast<GameCard*>(target.ptr);
    if (!card) return;
    card->AddStatus(CardStatus::HEXPROOF);
    std::cout << "🛡️ " << card->GetName() << " gains hexproof.\n";
}


void RegisterEffectFunctions_Keywords() {
    std::cout << "📦 Registering keyword effects...\n";
    EffectRegistry::Instance().Register("grant_keyword", GrantKeyword);
    EffectRegistry::Instance().Register("apply_silence", ApplySilence);
    EffectRegistry::Instance().Register("apply_hexproof", ApplyHexproof);
    EffectRegistry::Instance().Register("apply_haste", ApplyHaste);
}