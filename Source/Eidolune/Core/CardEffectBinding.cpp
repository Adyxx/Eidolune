

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
    return "Binding(" + (BoundEffect ? BoundEffect->ToString() : "NoEffect") + ")";
}

void CardEffectBinding::Validate() const {
    if (!BoundEffect) throw std::runtime_error("Missing effect in binding");
    if (!BoundTrigger) throw std::runtime_error("Missing trigger in binding");
}

std::shared_ptr<GameCard> CardEffectBinding::GetEventGameCard() {
    if (auto ptr = EventGameCard.lock()) {
        return ptr;
    }
    throw std::runtime_error("EventGameCard not set");
}
