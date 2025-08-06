

#include "CardEffectBinding.h"
#include "Effect.h"

CardEffectBinding::CardEffectBinding(std::shared_ptr<Card> card,
                                     std::shared_ptr<Trigger> trigger,
                                     std::shared_ptr<Effect> effect,
                                     std::shared_ptr<Condition> condition,
                                     std::optional<int> value,
                                     std::optional<TargetSpec> targetSpec)
    : ParentCard(card),
      BoundTrigger(trigger),
      BoundEffect(effect),
      BoundCondition(condition),
      Value(value),
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
