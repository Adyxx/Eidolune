#pragma once
#include <nlohmann/json.hpp>
#include <memory>
#include <vector>
#include "UserCharacterData.h"
#include "UserCardData.h"
#include "PullTracker.h"

class Deck;

struct UserData {
    int UserId;
    std::vector<UserCharacterData> Characters;
    std::vector<UserCardData> Cards;
    std::vector<std::shared_ptr<Deck>> Decks;

    std::unordered_map<int, GachaPullTracker> GachaTrackers;

    nlohmann::json ToJson() const;
    static UserData FromJson(const nlohmann::json& j);

    void SyncCharacter(const UserCharacterData& character);
    void SyncDeck(const std::shared_ptr<Deck>& deck);
    void SyncCard(const UserCardData& newCard);
    
    void RemoveDeckById(int deckId);
};
