

#include "CardEffectBinding.h"
#include "Effect.h"

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