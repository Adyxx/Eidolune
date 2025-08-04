#include "CardRegistry.h"

CardRegistry& CardRegistry::Instance() {
    static CardRegistry instance;
    return instance;
}

void CardRegistry::Register(int id, std::shared_ptr<Card> card) {
    if (card) {
        Cards[id] = card;
    }
}

std::shared_ptr<Card> CardRegistry::Get(int id) const {
    auto it = Cards.find(id);
    if (it != Cards.end())
        return it->second;
    return nullptr;
}

std::vector<Card*> CardRegistry::GetCardsByAuxType(AuxiliaryCardType type) {
    std::vector<Card*> result;
    const auto& cards = CardRegistry::Instance().GetAll();  // get the full map

    for (const auto& [id, cardPtr] : cards) {
        if (cardPtr && cardPtr->AuxilaryType == type) {
            result.push_back(cardPtr.get());
        }
    }
    return result;
}
