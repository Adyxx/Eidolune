#pragma once
#include <memory>
#include <string>
#include "TriggerRegistry.h"
#include "EffectRegistry.h"
#include "ConditionRegistry.h"
#include "CardRegistry.h" 
#include "Types.h"  

class KeywordEffectTemplate {
public:
    int Id;
    std::string Name;

    std::shared_ptr<Trigger> TriggerPtr;
    std::shared_ptr<Effect> EffectPtr;

    
    std::string RawValue;           // raw string value ("PIERCE", "3", etc)
    DynamicValueType ValueType;

    std::shared_ptr<Condition> ConditionPtr;
    std::optional<int> ConditionValue;
    std::optional<int> StaticValue;
    TargetSpec Targeting;
    TargetingRule TargetingRuleValue;

    ListeningZone Zone;
    TriggerScope Scope;

    std::shared_ptr<class Card> LinkedCard;

    KeywordEffectTemplate(
        int id,
        std::string name,
        std::shared_ptr<Trigger> trigger_,
        std::shared_ptr<Effect> effect_,
        std::string rawValue_,
        DynamicValueType valueType_,
        std::shared_ptr<Condition> condition_,
        std::optional<int> conditionValue_,
        TargetSpec targeting_,
        TargetingRule targetingRule_,
        ListeningZone zone_,
        TriggerScope scope_,
        std::shared_ptr<Card> linkedCard_,
        std::optional<int> staticValue_ = std::nullopt
    )
        : Id(id),
          Name(std::move(name)),
          TriggerPtr(std::move(trigger_)),
          EffectPtr(std::move(effect_)),
          RawValue(std::move(rawValue_)),
          ValueType(valueType_),
          ConditionPtr(std::move(condition_)),
          ConditionValue(conditionValue_),
          StaticValue(staticValue_),
          Targeting(targeting_),
          TargetingRuleValue(targetingRule_),
          Zone(zone_),
          Scope(scope_),
          LinkedCard(std::move(linkedCard_))
    {}


    std::shared_ptr<Card> GetLinkedCard() const;
    std::optional<int> GetConditionValue() const;
    std::shared_ptr<Trigger> GetTrigger() const;
    std::shared_ptr<Effect> GetEffect() const;
    std::optional<int> GetValue() const;
    DynamicValueType GetValueType() const;
    std::optional<TargetSpec> GetTargetSpec() const;
    std::shared_ptr<Condition> GetCondition() const;
    TriggerScope GetScope() const;
    ListeningZone GetZone() const;
    CardZone GetZoneAsCardZone() const;
    std::string GetRawValue() const;
};
