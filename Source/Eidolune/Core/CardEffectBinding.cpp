

#include "CardEffectBinding.h"
#include "Trigger.h"
#include "Effect.h"
#include "GameCard.h"

CardEffectBinding::CardEffectBinding(std::shared_ptr<Card> card,
                                     std::shared_ptr<Trigger> trigger,
                                     std::shared_ptr<Effect> effect,
                                     std::shared_ptr<Condition> condition,
                                     const std::string& rawValue,
                                     DynamicValueType valueType,
                                     std::optional<int> staticValue,
                                     std::optional<TargetSpec> targetSpec)
    : ParentCard(card),
      BoundTrigger(trigger),
      BoundEffect(effect),
      BoundCondition(condition),
      RawValue(rawValue),
      ValueType(valueType),
      StaticValue(staticValue),
      Targeting(targetSpec) {}

std::string CardEffectBinding::ToString() const {
    try {
        if (!BoundEffect) return "Binding(NoEffect)";
        return "Binding(" + BoundEffect->ToString() + ")";
    } catch (...) {
        return "Binding(Effect: <EXCEPTION>)";
    }
}

std::shared_ptr<GameCard> CardEffectBinding::GetEventGameCard() {
    if (auto ptr = EventGameCard.lock()) {
        return ptr;
    }
    throw std::runtime_error("EventGameCard not set");
}

CardZone CardEffectBinding::GetZoneAsCardZone() const {
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

    std::shared_ptr<Card> CardEffectBinding::GetLinkedCard() const { return LinkedCard; }
    std::optional<int> CardEffectBinding::GetConditionValue() const { return ConditionValue; }
    std::shared_ptr<Trigger> CardEffectBinding::GetTrigger() const { return BoundTrigger; }
    std::shared_ptr<Card> CardEffectBinding::GetEventCard() const { return ParentCard; }
    std::shared_ptr<Effect> CardEffectBinding::GetEffect() const { return BoundEffect; }
    std::optional<int> CardEffectBinding::GetValue() const { return StaticValue; }
    std::string CardEffectBinding::GetRawValue() const { return RawValue; }
    DynamicValueType CardEffectBinding::GetValueType() const { return ValueType; }
    std::optional<TargetSpec> CardEffectBinding::GetTargetSpec() const { return Targeting; }
    std::shared_ptr<Condition> CardEffectBinding::GetCondition() const { return BoundCondition; }
    std::shared_ptr<Card> CardEffectBinding::GetCard() const { return ParentCard; }
    TriggerScope CardEffectBinding::GetScope() const { return Scope; }
    ListeningZone CardEffectBinding::GetZone() const { return Zone; } 

    void CardEffectBinding::SetZone(ListeningZone z) { Zone = z; }
    void CardEffectBinding::SetScope(TriggerScope s) { Scope = s; }
    void CardEffectBinding::SetTargetingRule(TargetingRule tr) { targetingRule = tr; }
    void CardEffectBinding::SetCondition(std::shared_ptr<Condition> condition) { BoundCondition = condition; }
    void CardEffectBinding::SetConditionValue(int value) { ConditionValue = value; }
    void CardEffectBinding::SetLinkedCard(std::shared_ptr<Card> linked) { LinkedCard = linked; }

    bool CardEffectBinding::HasZone() const { return Zone != ListeningZone::ANY; }
