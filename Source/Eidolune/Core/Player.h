#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Deck.h"
//#include "GameCard.h"

class GameCard;

class Player {
public:
    int ID = -1; // note for the future.. this is not being set anywhere (yet)
    std::shared_ptr<Deck> DeckRef;
    std::vector<std::shared_ptr<GameCard>> Hand;
    std::vector<std::shared_ptr<GameCard>> Board;
    std::vector<std::shared_ptr<GameCard>> Graveyard;
    std::vector<std::shared_ptr<GameCard>> DrawPile;

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

    virtual ~Player() = default; 
    Player() = default;
};
