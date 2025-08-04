#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Deck.h"
//#include "GameCard.h"
#include "Types.h"
#include "Card.h"

class GameCard;

class Player {
public:
    int ID = -1; 
    std::shared_ptr<Deck> DeckRef;
    std::vector<std::shared_ptr<GameCard>> Hand;
    std::vector<std::shared_ptr<GameCard>> Board;
    std::vector<std::shared_ptr<GameCard>> Graveyard;
    std::vector<std::shared_ptr<GameCard>> DrawPile;

    int TurnCount = 0;
    int Health = 20;
    int MaxHealth = 20;
    int Energy = 0;
    int MaxEnergy = 0;
    int MaxHandSize = 10;
    int PlayerIndex = 0;

    int CardsPlayedThisTurn = 0;
    
    int GetTurnNumber() const {
        return TurnCount;
    }

    Player(std::shared_ptr<Deck> deck, int index);

    std::string GetName() const;

    void StartTurn();
    void EndTurn();

    Player* Opponent = nullptr;
    Player* GetOpponent() const { return Opponent; } 

    virtual ~Player() = default; 
    Player() = default;

    std::unordered_map<AuxiliaryCardType, std::vector<Card*>> ChosenAuxiliaryCards;

    bool HasChosenAuxCard(Card* card) const {
        auto it = ChosenAuxiliaryCards.find(card->AuxilaryType);
        if (it == ChosenAuxiliaryCards.end()) return false;

        return std::find(it->second.begin(), it->second.end(), card) != it->second.end();
    }

    void AddChosenAuxCard(Card* card) {
        ChosenAuxiliaryCards[card->AuxilaryType].push_back(card);
    }
    void PromptAuxCardChoice(AuxiliaryCardType type, const std::vector<Card*>& options);

};
