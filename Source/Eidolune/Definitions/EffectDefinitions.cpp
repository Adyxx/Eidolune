
#include "EffectRegistry.h"
#include <iostream>

void ApplyHaste(void* source, void* target, std::optional<int> value) {
    std::cout << "[Effect] ApplyHaste triggered\n";
}

void DrawCard(void* source, void* target, std::optional<int> value) {
    std::cout << "[Effect] DrawCard triggered, drawing " << (value.value_or(1)) << " cards\n";
}

void OverrideDeckLimit(void* source, void* target, std::optional<int> value) {
    std::cout << "[Effect] OverrideDeckLimit to " << (value.value_or(0)) << "\n";
}

void DealDamage(void* source, void* target, std::optional<int> value) {
    std::cout << "[Effect] DealDamage for " << (value.value_or(0)) << "\n";
}

void InitEffectRegistry() {
    EffectRegistry::Register("apply_haste", { ApplyHaste, TargetHint::CARD });
    EffectRegistry::Register("draw_card", { DrawCard, TargetHint::PLAYER });
    EffectRegistry::Register("override_deck_limit", { OverrideDeckLimit, TargetHint::CARD });
    EffectRegistry::Register("deal_damage", { DealDamage, TargetHint::NONE });
}
