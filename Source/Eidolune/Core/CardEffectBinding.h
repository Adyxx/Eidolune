
#pragma once
#include <string>
#include <memory>
#include <optional>
#include <stdexcept>
#include "Types.h"

class Card;
class Condition;
class Trigger;
class Effect;
class GameCard;

class CardEffectBinding {
public:
    std::shared_ptr<Card> ParentCard;
    std::shared_ptr<Trigger> BoundTrigger;
    std::shared_ptr<Effect> BoundEffect;
    std::shared_ptr<Condition> BoundCondition;
    
    std::string RawValue; 
    DynamicValueType ValueType = DynamicValueType::UNKNOWN;
    std::optional<int> StaticValue;
    std::optional<int> ConditionValue;

    std::optional<TargetSpec> Targeting;
    ListeningZone Zone = ListeningZone::ANY;
    TriggerScope Scope = TriggerScope::SELF;
    TargetingRule targetingRule = TargetingRule::NONE;
    std::shared_ptr<Card> LinkedCard;


    std::shared_ptr<Card> GetLinkedCard() const;
    std::optional<int> GetConditionValue() const;
    std::shared_ptr<Trigger> GetTrigger() const;
    std::shared_ptr<Card> GetEventCard() const;
    std::shared_ptr<Effect> GetEffect() const;
    std::optional<int> GetValue() const;
    DynamicValueType GetValueType() const;
    std::optional<TargetSpec> GetTargetSpec() const;
    std::shared_ptr<Condition> GetCondition() const;
    std::shared_ptr<Card> GetCard() const;
    TriggerScope GetScope() const;
    ListeningZone GetZone() const;
    CardZone GetZoneAsCardZone() const;
    std::string GetRawValue() const;
    
    
    void SetZone(ListeningZone z);
    void SetScope(TriggerScope s);
    void SetTargetingRule(TargetingRule tr);
    void SetCondition(std::shared_ptr<Condition> condition);
    void SetConditionValue(int value);
    void SetLinkedCard(std::shared_ptr<Card> linked);

    bool HasZone() const;

    CardEffectBinding(std::shared_ptr<Card> card,
                    std::shared_ptr<Trigger> trigger,
                    std::shared_ptr<Effect> effect,
                    std::shared_ptr<Condition> condition,
                    const std::string& rawValue,
                    DynamicValueType valueType,
                    std::optional<int> staticValue = std::nullopt,
                    std::optional<TargetSpec> targetSpec = std::nullopt);


    std::string ToString() const;
    std::weak_ptr<GameCard> EventGameCard;
    std::shared_ptr<GameCard> GetEventGameCard(); 

};
