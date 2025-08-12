
#pragma once

#include <functional>
#include <memory>
#include <string>
#include <optional>

class Card;
class CardEffectBinding;
class GameCard;
class Player;
class Effect;
struct Target;

#include "Types.h"

class TriggerBuilder {
public:
    static std::function<void(std::unordered_map<std::string, void*>)> Build(std::shared_ptr<CardEffectBinding> binding);
    
    static Target ResolveTarget(std::shared_ptr<CardEffectBinding> binding, Player* triggerOwner, GameCard* eventCard, std::function<void(void*, Target, std::optional<int>)> effectFunc, std::optional<int> resolvedValue);
private:
    static bool IsSilenced(GameCard* card);
    static std::optional<int> ResolveValue(std::shared_ptr<CardEffectBinding> binding, Player* triggerOwner);
    static bool IsTriggerScopeMatch(GameCard* eventCard, GameCard* triggerCard, TriggerScope scope);

};
