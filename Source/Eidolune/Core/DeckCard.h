
#pragma once
#include <memory>
#include <string>
#include <vector>

class Deck;
class Card;
class GameCard;

class DeckCard {
public:
    std::shared_ptr<Deck> ParentDeck;
    std::shared_ptr<Card> CardRef;
    int Quantity;

    //std::shared_ptr<GameCard> GameCardInstance;
    std::vector<std::shared_ptr<GameCard>> GameCardCopies;

    DeckCard(std::shared_ptr<Deck> deck, std::shared_ptr<Card> card, int qty = 1);

    std::string ToString() const;

    std::shared_ptr<Card> CreateCardInstance() const;
};
