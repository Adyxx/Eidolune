#include "DeckLoader.h"
#include "../Registry/DeckRegistry.h"
#include "../Registry/UserRegistry.h"
#include "../Registry/CharacterRegistry.h"
#include "../Registry/CardRegistry.h"
#include "../Core/DeckCard.h"
#include "../Http/ApiClient.h" 
#include <iostream>

#include "../Core/Deck.h"

std::unordered_map<int, std::shared_ptr<Deck>> DeckLoader::LoadAll() {
    std::cout << "🔄 Loading Decks from API...\n";

    std::unordered_map<int, std::shared_ptr<Deck>> result;
    json decksJson = ApiClient::Get("decks/");


    int loadedCount = 0;
    int skippedCount = 0;

    for (const auto& d : decksJson) {
        if (!d.contains("id") || !d.contains("owner") || !d.contains("main_character") || !d.contains("name")) {
            std::cerr << "❌ Skipping deck — missing required fields: " << d.dump() << "\n";
            skippedCount++;
            continue;
        }

        try {
            int deckId = d["id"].get<int>();
            int ownerId = d["owner"].get<int>();
            int mainCharId = d["main_character"].get<int>();

            auto owner = UserRegistry::Instance().Get(ownerId);
            if (!owner) {
                std::cerr << "⚠️  Deck '" << d["name"] << "' has unknown owner ID: " << ownerId << "\n";
            }

            auto mainChar = CharacterRegistry::Instance().Get(mainCharId);

            std::shared_ptr<Character> partner = nullptr;

            if (d.contains("partner_character") && !d["partner_character"].is_null()) {
                partner = CharacterRegistry::Instance().Get(d["partner_character"].get<int>());
            }

            auto deck = std::make_shared<Deck>(
                owner,
                d["name"].get<std::string>(),
                mainChar,
                partner,
                d.value("description", "")
            );

            result[deckId] = deck;
            DeckRegistry::Instance().Register(deckId, deck);

            loadedCount++;
        } catch (const std::exception& e) {
            std::cerr << "❌ Error loading deck: " << e.what() << " in entry: " << d.dump() << "\n";
            skippedCount++;
        }
    }

    std::cout << "✅ Loaded " << loadedCount << " decks successfully.\n";
    if (skippedCount > 0) {
        std::cout << "⚠️ Skipped " << skippedCount << " decks due to errors or missing data.\n";
    }

    return result;
}
