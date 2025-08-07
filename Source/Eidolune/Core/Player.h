#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

//#include "GameCard.h"
#include "Types.h"
//#include "Card.h"


class Card;
class Deck;
class GameCard;
class TriggerObserver;

class Player {
public:
    int ID = -1; 
    std::shared_ptr<Deck> DeckRef;
    std::vector<std::shared_ptr<GameCard>> Hand;
    std::vector<std::shared_ptr<GameCard>> Board;
    std::vector<std::shared_ptr<GameCard>> Graveyard;
    std::vector<std::shared_ptr<GameCard>> DrawPile;
    
    std::vector<std::shared_ptr<GameCard>> ExileZone;
    std::vector<std::shared_ptr<GameCard>> OathZone;
    

    int TurnCount = 0;
    int Health = 20;
    int MaxHealth = 20;
    int Energy = 0;
    int MaxEnergy = 0;
    int MaxHandSize = 10;
    int PlayerIndex = 0;
    bool DealtDamageToEnemyHeroThisTurn = false;

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


    std::shared_ptr<TriggerObserver> Observer;

    std::shared_ptr<TriggerObserver> GetTriggerObserver() const {
        return Observer;
    }


    std::unordered_map<AuxiliaryCardType, std::vector<std::shared_ptr<Card>>> ChosenAuxiliaryCards;


    bool HasChosenAuxCard(const std::shared_ptr<Card>& card) const;
    void AddChosenAuxCard(std::shared_ptr<Card> card);
    void PromptAuxCardChoice(AuxiliaryCardType type, const std::vector<std::shared_ptr<Card>>& options);

    int PromptChooseOption(const std::vector<std::string>& options);
};
