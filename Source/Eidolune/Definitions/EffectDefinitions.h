

#pragma once
#include <optional>

void DrawCard(void* source, void* target, std::optional<int> value);
void ApplyHaste(void* source, void* target, std::optional<int> value);
void OverrideDeckLimit(void* source, void* target, std::optional<int> value);
void DealDamage(void* source, void* target, std::optional<int> value);
void InitEffectRegistry();
