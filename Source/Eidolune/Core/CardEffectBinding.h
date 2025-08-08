
#pragma once
#include <string>
#include <memory>
#include <optional>
#include <stdexcept>
#include "GameCard.h"
#include "Types.h"
//#include "EffectRegistry.h"
//#include "TriggerRegistry.h"

class Card;
//class Trigger;
//class Effect;
class Condition;

#include "Trigger.h"
#include "Effect.h"

class CardEffectBinding {
public:
    std::shared_ptr<Card> ParentCard;
    std::shared_ptr<Trigger> BoundTrigger;
    std::shared_ptr<Effect> BoundEffect;
    std::shared_ptr<Condition> BoundCondition;

    //std::optional<int> Value;
    
    std::string RawValue; // direct from JSON
    DynamicValueType ValueType = DynamicValueType::UNKNOWN;
    std::optional<int> StaticValue;

    
    std::optional<TargetSpec> Targeting;

    ListeningZone Zone = ListeningZone::ANY;
    TriggerScope Scope = TriggerScope::SELF;

    ////////////////
    std::shared_ptr<Card> LinkedCard;
    std::optional<int> ConditionValue;
    std::shared_ptr<Card> GetLinkedCard() const { return LinkedCard; }
    void SetLinkedCard(std::shared_ptr<Card> linked) { LinkedCard = linked; }
    std::optional<int> GetConditionValue() const { return ConditionValue; }
    void SetConditionValue(int value) { ConditionValue = value; }
    ////////////////

    std::shared_ptr<Trigger> GetTrigger() const { return BoundTrigger; }
    std::shared_ptr<Card> GetEventCard() const { return ParentCard; }
    std::shared_ptr<Effect> GetEffect() const { return BoundEffect; }
    
    std::optional<int> GetValue() const { return StaticValue; }
    DynamicValueType GetValueType() const { return ValueType; }

    std::optional<TargetSpec> GetTargetSpec() const { return Targeting; }
    std::shared_ptr<Condition> GetCondition() const { return BoundCondition; }
    std::shared_ptr<Card> GetCard() const { return ParentCard; }
    TriggerScope GetScope() const { return Scope; }


    bool HasZone() const { return Zone != ListeningZone::ANY; }
    CardZone GetZoneAsCardZone() const {
        switch (Zone) {
            case ListeningZone::BOARD: return CardZone::BOARD;
            case ListeningZone::HAND: return CardZone::HAND;
            case ListeningZone::DECK: return CardZone::DECK;
            case ListeningZone::GRAVEYARD: return CardZone::GRAVEYARD;
            case ListeningZone::ANY: return CardZone::UNKNOWN;
            case ListeningZone::OATHZONE: return CardZone::OATHZONE;
            default: throw std::runtime_error("Unhandled ListeningZone in GetZoneAsCardZone()");
        }
    }

    ListeningZone GetZone() const { return Zone; } 

    void SetZone(ListeningZone z) { Zone = z; }
    void SetScope(TriggerScope s) { Scope = s; }
    void SetCondition(std::shared_ptr<Condition> condition) {
        BoundCondition = condition;
    }


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
