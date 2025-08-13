#pragma once

struct Target;

void RegisterEffectFunctions_Deck();
void DrawCard(void* source, Target target, std::optional<int> value);
