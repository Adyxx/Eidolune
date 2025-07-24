#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include "Card.h"

class Player;

class GameCard : public std::enable_shared_from_this<GameCard> {
public:
    std::shared_ptr<Card> Model;

    Player* Owner = nullptr;
    std::string Zone = "deck";
    bool SummoningSickness = true;
    bool Tapped = false;
    int DamageTaken = 0;

    std::vector<std::string> TempBuffs;

    GameCard(std::shared_ptr<Card> model);
    
    std::string GetName() const;
    int GetCost() const;
    std::optional<int> GetPower() const;
    std::optional<int> GetHealth() const;

    int CurrentHealth() const;
    std::string ToString() const;
};
