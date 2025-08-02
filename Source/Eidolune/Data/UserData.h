#pragma once
#include <nlohmann/json.hpp>
#include <memory>
#include <vector>
#include "UserCharacterData.h"

class Deck;

struct UserData {
    int UserId;
    std::vector<UserCharacterData> Characters;
    std::vector<std::shared_ptr<Deck>> Decks;

    nlohmann::json ToJson() const;
    static UserData FromJson(const nlohmann::json& j);

    void SyncCharacter(const UserCharacterData& character);

};
