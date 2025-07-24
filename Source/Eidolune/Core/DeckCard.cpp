

#include "DeckCard.h"
#include "Card.h"

DeckCard::DeckCard(std::shared_ptr<Deck> deck, std::shared_ptr<Card> card, int qty)
        : ParentDeck(deck), CardRef(card), Quantity(qty) {}

std::string DeckCard::ToString() const {
    return CardRef->ToString() + " x" + std::to_string(Quantity);
}