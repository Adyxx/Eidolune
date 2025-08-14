#pragma once
#include "../Abilities/Other/AbilityHelpers/BloodboundNode.h"
#include <unordered_map>

void RegisterBloodboundAbilities(std::unordered_map<int, BloodboundNode>& outNodes);
