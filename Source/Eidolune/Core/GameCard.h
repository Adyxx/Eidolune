#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

#include "Card.h"
#include "Types.h"

class Player;
class CardEffectBinding;

struct AuraEffect {
    int AttackBuff;
    int HealthBuff;
};


inline CardStatus operator|(CardStatus a, CardStatus b) {
    return static_cast<CardStatus>(static_cast<int>(a) | static_cast<int>(b));
}
inline CardStatus operator&(CardStatus a, CardStatus b) {
    return static_cast<CardStatus>(static_cast<int>(a) & static_cast<int>(b));
}
inline CardStatus& operator|=(CardStatus& a, CardStatus b) {
    a = a | b;
    return a;
}

class GameCard : public std::enable_shared_from_this<GameCard> {
public:
    int Id;
    std::shared_ptr<Card> Model;
    Player* Owner = nullptr;
    CardZone Zone = CardZone::DECK;
    bool SummoningSickness = true;
    bool Tapped = false;
    int DamageTaken = 0;
    int TimeCounter = 0;

    std::unordered_map<int, AuraEffect> ActiveAuras;
    std::unordered_map<int, int> ActiveCostModifiers;

    GameCard(std::shared_ptr<Card> model);

    std::string GetName() const;
    int GetCost() const;
    std::optional<int> GetPower() const;
    std::optional<int> GetHealth() const;

    int BaseAttack() const;
    int BaseHealth() const;

    int CurrentAttack() const;
    int MaxHealth() const;
    int CurrentHealth() const;

    void ApplyAura(int sourceId, int atkBuff, int hpBuff);
    void RemoveAura(int sourceId);
    virtual ~GameCard() = default;

    std::string ToString() const;

    CardStatus StatusFlags = CardStatus::NONE;

    bool HasStatus(CardStatus s) const {
        return (StatusFlags & s) != CardStatus::NONE;
    }

    void AddStatus(CardStatus s) {
        StatusFlags |= s;
    }

    void RemoveStatus(CardStatus s) {
        StatusFlags = static_cast<CardStatus>(
            static_cast<int>(StatusFlags) & ~static_cast<int>(s)
        );
    }

    bool CanBeTargetedBy(const GameCard* source) const {
        if (HasStatus(CardStatus::HEXPROOF) && source && source->Owner != this->Owner) {
            return false;
        }
        return true;
    }

    bool CanBeAffectedBy(const GameCard* source) const {
        if (HasStatus(CardStatus::INVULNERABLE)) {
            return false;
        }
        return true;
    }

    std::vector<std::shared_ptr<CardEffectBinding>> RuntimeEffectBindings;

    void ApplyCostModifier(int sourceId, int delta) {
        ActiveCostModifiers[sourceId] = delta;
    }

    void RemoveCostModifier(int sourceId) {
        ActiveCostModifiers.erase(sourceId);
    }

    int CurrentCost() const {
        int baseCost = Model->Cost; 
        int modSum = 0;
        for (auto& [_, delta] : ActiveCostModifiers) {
            modSum += delta;
        }
        return std::max(0, baseCost + modSum);
    }


    std::weak_ptr<GameCard> AttachedTo; // If this card is attached to another
    std::vector<std::shared_ptr<GameCard>> Attachments; // If this card has assets attached

};