

#pragma once
#include <optional>
#include "EffectRegistry.h"
#include "../Core/DeckCard.h"
#include "../Core/Player.h"
#include "../Core/GameCard.h"
#include <iostream>
#include "TriggerObserver.h"
#include "GameActions.h"

void DrawCard(void* source, Target target, std::optional<int> value);
void ApplyHaste(void* source, Target  target, std::optional<int> value);
void OverrideDeckLimit(void* source, Target  target, std::optional<int> value);
void DealDamage(void* source, Target  target, std::optional<int> value);


void RegisterEffectFunctions();