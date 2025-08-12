
#include "KeywordEffectTemplate.h"


    std::shared_ptr<Card> KeywordEffectTemplate::GetLinkedCard() const { return LinkedCard; }
    std::optional<int> KeywordEffectTemplate::GetConditionValue() const { return ConditionValue; }
    std::shared_ptr<Trigger> KeywordEffectTemplate::GetTrigger() const { return TriggerPtr; }

    std::shared_ptr<Effect> KeywordEffectTemplate::GetEffect() const { return EffectPtr; }
    std::optional<int> KeywordEffectTemplate::GetValue() const { return StaticValue; }
    std::string KeywordEffectTemplate::GetRawValue() const { return RawValue; }
    DynamicValueType KeywordEffectTemplate::GetValueType() const { return ValueType; }
    std::optional<TargetSpec> KeywordEffectTemplate::GetTargetSpec() const { return Targeting; }
    std::shared_ptr<Condition> KeywordEffectTemplate::GetCondition() const { return ConditionPtr; }

    TriggerScope KeywordEffectTemplate::GetScope() const { return Scope; }
    ListeningZone KeywordEffectTemplate::GetZone() const { return Zone; } 
