#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Deck.h"

class GameCard;

class Player {
public:
    std::shared_ptr<Deck> DeckRef;
    std::vector<std::shared_ptr<GameCard>> Hand;
    std::vector<std::shared_ptr<GameCard>> Board;
    std::vector<std::shared_ptr<GameCard>> Graveyard;

    int Health = 20;
    int MaxHealth = 20;
    int Energy = 0;
    int MaxEnergy = 0;
    int MaxHandSize = 10;
    int PlayerIndex = 0;

    Player(std::shared_ptr<Deck> deck, int index);

    std::string GetName() const;

    void StartTurn();
    void EndTurn();

    Player* Opponent = nullptr;
    Player* GetOpponent() const { return Opponent; } 
};
