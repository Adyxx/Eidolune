
#pragma once
#include <memory>
#include <string>

class Deck;
class Card;

class DeckCard {
public:
    std::shared_ptr<Deck> ParentDeck;
    std::shared_ptr<Card> CardRef;
    int Quantity;

    DeckCard(std::shared_ptr<Deck> deck, std::shared_ptr<Card> card, int qty = 1)
        : ParentDeck(deck), CardRef(card), Quantity(qty) {}

    std::string ToString() const;
};
