
#pragma once
#include <string>
#include <memory>
#include <optional>

enum class TargetSpec {
    ENEMY_BOARD,
    ENEMY_HERO,
    ENEMY_BOARD_OR_HERO,
    FRIENDLY_BOARD,
    SELF,
    ANY,
    UNKNOWN
};

class Card;
class Trigger;
class Effect;
class Condition;
class Restriction;

class CardEffectBinding {
public:
    std::shared_ptr<Card> ParentCard;
    std::shared_ptr<Trigger> BoundTrigger;
    std::shared_ptr<Effect> BoundEffect;
    std::shared_ptr<Condition> BoundCondition;
    std::shared_ptr<Restriction> BoundRestriction;

    std::optional<int> Value;
    std::optional<TargetSpec> Targeting;

    CardEffectBinding(std::shared_ptr<Card> card,
                      std::shared_ptr<Trigger> trigger,
                      std::shared_ptr<Effect> effect,
                      std::shared_ptr<Condition> condition = nullptr,
                      std::shared_ptr<Restriction> restriction = nullptr,
                      std::optional<int> value = std::nullopt,
                      std::optional<TargetSpec> targetSpec = std::nullopt)
        : ParentCard(card), BoundTrigger(trigger), BoundEffect(effect),
          BoundCondition(condition), BoundRestriction(restriction),
          Value(value), Targeting(targetSpec) {}

    std::string ToString() const;
    void Validate() const;
};
