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

std::vector<std::shared_ptr<Card>> CardRegistry::GetCardsByAuxType(AuxiliaryCardType type) {
    std::vector<std::shared_ptr<Card>> result;
    const auto& cards = CardRegistry::Instance().GetAll();  

    for (const auto& [id, cardPtr] : cards) {
        if (cardPtr && cardPtr->AuxilaryType == type) {
            result.push_back(cardPtr);  // Keep as shared_ptr, not .get()
        }
    }

    return result;
}
