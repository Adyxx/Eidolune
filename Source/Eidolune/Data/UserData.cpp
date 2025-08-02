#include "UserData.h"
#include "UserCharacterData.h"
#include "Deck.h"

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

    return j;
}

UserData UserData::FromJson(const nlohmann::json& j) {
    UserData data;
    data.UserId = j.value("user_id", -1);

    if (j.contains("characters") && j["characters"].is_array()) {
        for (const auto& charJson : j["characters"]) {
            data.Characters.push_back(UserCharacterData::FromJson(charJson));
        }
    }

    if (j.contains("decks") && j["decks"].is_array()) {
        for (const auto& deckJson : j["decks"]) {
            auto deck = Deck::FromJson(deckJson);
            if (deck) data.Decks.push_back(deck);
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
