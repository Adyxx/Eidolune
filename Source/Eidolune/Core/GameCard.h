#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

#include "Card.h"
#include "Types.h"

class Player;

struct AuraEffect {
    int AttackBuff;
    int HealthBuff;
};

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
    // Buffing aura tracking
    std::unordered_map<int, AuraEffect> ActiveAuras; // SourceCardID → Effect

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
};