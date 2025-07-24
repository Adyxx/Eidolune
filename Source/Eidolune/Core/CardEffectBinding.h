
#pragma once
#include <string>
#include <memory>
#include <optional>
#include <stdexcept>
#include "GameCard.h"

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

    std::optional<int> Value;
    std::optional<TargetSpec> Targeting;

    std::shared_ptr<Trigger> GetTrigger() const { return BoundTrigger; }
    std::shared_ptr<Card> GetEventCard() const { return ParentCard; }
    std::shared_ptr<Effect> GetEffect() const { return BoundEffect; }
    std::optional<int> GetValue() const { return Value; }
    std::optional<TargetSpec> GetTargetSpec() const { return Targeting; }
    std::shared_ptr<Condition> GetCondition() const { return BoundCondition; }
    std::shared_ptr<Card> GetCard() const { return ParentCard; }


    CardEffectBinding(std::shared_ptr<Card> card,
                      std::shared_ptr<Trigger> trigger,
                      std::shared_ptr<Effect> effect,
                      std::shared_ptr<Condition> condition = nullptr,
                      std::optional<int> value = std::nullopt,
                      std::optional<TargetSpec> targetSpec = std::nullopt);

    std::string ToString() const;

    std::weak_ptr<GameCard> EventGameCard;

    std::shared_ptr<GameCard> GetEventGameCard(); 

    void Validate() const;
};
