
#include "DeckCardLoader.h"
#include "../Http/ApiClient.h"
#include "../Registry/DeckRegistry.h"
#include "../Registry/CardRegistry.h"
#include <iostream>

std::unordered_map<int, std::shared_ptr<DeckCard>> DeckCardLoader::LoadAll() {

    std::cout << "🔄 Loading DeckCards from API...\n";
    auto deckCardsJson = ApiClient::Get("deckcards/");

    std::unordered_map<int, std::shared_ptr<DeckCard>> result;

    for (const auto& d : deckCardsJson) {
        int deckId = d["deck"];
        int cardId = d["card"];
        int qty = d["quantity"];

        auto deck = DeckRegistry::Instance().Get(deckId);
        auto card = CardRegistry::Instance().Get(cardId);
        if (!deck || !card) {
            std::cerr << "Missing deck or card for deckCard. Deck ID: " << deckId << ", Card ID: " << cardId << "\n";
            continue;
        }

        auto deckCard = std::make_shared<DeckCard>(deck, card, qty);
        deck->DeckCards.push_back(deckCard);
    }

    return result;
}
