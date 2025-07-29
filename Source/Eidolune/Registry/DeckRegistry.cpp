#include "DeckRegistry.h"

DeckRegistry& DeckRegistry::Instance() {
    static DeckRegistry instance;
    return instance;
}

void DeckRegistry::Register(int id, std::shared_ptr<Deck> deck) {
    if (deck) {
        Decks[id] = deck;
    }
}

std::shared_ptr<Deck> DeckRegistry::Get(int id) const {
    auto it = Decks.find(id);
    return (it != Decks.end()) ? it->second : nullptr;
}
