#include "UserData.h"
#include "UserCharacterData.h"
#include "Deck.h"
#include "UserRegistry.h"
#include <iostream>

nlohmann::json UserData::ToJson() const {
    nlohmann::json j;
    j["user_id"] = UserId;

    j["characters"] = nlohmann::json::array();
    for (const auto& c : Characters) {
        j["characters"].push_back(c.ToJson());
    }

    j["decks"] = nlohmann::json::array();
    for (const auto& d : Decks) {
        j["decks"].push_back(d->ToJson());
    }

    j["owned_cards"] = nlohmann::json::array();
    for (const auto& c : Cards) {
        j["owned_cards"].push_back(c.ToJson());
    }

    return j;
}

UserData UserData::FromJson(const nlohmann::json& j) {
    UserData data;
    data.UserId = j.value("user_id", -1);

    auto owner = UserRegistry::Instance().Get(data.UserId);
    if (!owner) {
        std::cerr << "❌ Could not find user with ID " << data.UserId << " in registry.\n";
        return data;
    }

    if (j.contains("characters") && j["characters"].is_array()) {
        for (const auto& charJson : j["characters"]) {
            data.Characters.push_back(UserCharacterData::FromJson(charJson));
        }
    }

    if (j.contains("decks") && j["decks"].is_array()) {
        for (const auto& deckJson : j["decks"]) {
            auto deck = Deck::FromJson(deckJson, owner);
            if (deck) data.Decks.push_back(deck);
        }
    }

    if (j.contains("owned_cards") && j["owned_cards"].is_array()) {
        for (const auto& cardJson : j["owned_cards"]) {
            data.Cards.push_back(UserCardData::FromJson(cardJson));
        }
    }

    return data;
}


void UserData::SyncCharacter(const UserCharacterData& character) {
    auto it = std::find_if(Characters.begin(), Characters.end(),
        [&character](const UserCharacterData& c) {
            return c.CharacterId == character.CharacterId;
        });

    if (it != Characters.end()) {
        *it = character;
    } else {
        Characters.push_back(character);
    }
}

void UserData::SyncDeck(const std::shared_ptr<Deck>& deck) {
    auto it = std::find_if(Decks.begin(), Decks.end(),
        [&deck](const std::shared_ptr<Deck>& d) {
            return d && deck && d->ID == deck->ID;
        });

    if (it != Decks.end()) {
        *it = deck;
    } else {
        Decks.push_back(deck);
    }
}

void UserData::RemoveDeckById(int deckId) {
    auto it = std::remove_if(Decks.begin(), Decks.end(),
        [deckId](const std::shared_ptr<Deck>& d) {
            return d && d->ID == deckId;
        });
    if (it != Decks.end()) {
        Decks.erase(it, Decks.end());
        std::cout << "🗑️ Removed deck with ID " << deckId << " from user data.\n";
    } else {
        std::cerr << "⚠️ Tried to remove non-existent deck ID " << deckId << " from user data.\n";
    }
}


void UserData::SyncCard(const UserCardData& newCard) {
    auto it = std::find_if(Cards.begin(), Cards.end(),
        [&newCard](const UserCardData& c) {
            return c.CardId == newCard.CardId;
        });

    if (it != Cards.end()) {
        it->Quantity += newCard.Quantity;
    } else {
        Cards.push_back(newCard);
    }
}
