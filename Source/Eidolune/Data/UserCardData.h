#pragma once
#include <memory>
#include <nlohmann/json.hpp>
#include "Card.h"

struct UserCardData {
    int CardId;
    std::shared_ptr<Card> ResolvedCard = nullptr;
    int Quantity = 1;
    bool IsLocked = false;

    nlohmann::json ToJson() const {
        return {
            {"card_id", CardId},
            {"quantity", Quantity},
            {"locked", IsLocked}
        };
    }

    static UserCardData FromJson(const nlohmann::json& j) {
        UserCardData data;
        data.CardId = j.at("card_id").get<int>();
        data.Quantity = j.value("quantity", 1);
        data.IsLocked = j.value("locked", false);
        return data;
    }

    static UserCardData FromCard(Card* card) {
        UserCardData data;
        data.CardId = card->ID;
        data.ResolvedCard = std::make_shared<Card>(*card);
        data.Quantity = 1;
        return data;
    }
};
