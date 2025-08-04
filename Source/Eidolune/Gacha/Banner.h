#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Types.h"
#include "Card.h"
#include "Currency.h"
#include "CardRegistry.h"
#include <nlohmann/json.hpp>

enum class BannerType {
    CARD,
    EQUIPMENT 
};

struct BannerCardEntry {
    std::shared_ptr<Card> Card;
    float Rate; 
    bool IsRateUp = false;
};

struct Banner {
    int Id;
    std::string Name;
    BannerType Type = BannerType::CARD;
    std::vector<BannerCardEntry> Pool;

    std::unordered_map<Rarity, float> BaseRates; 
    int HardPity = 80;
    int SoftPityStart = 60;

    CurrencyType PullCurrency;
    bool IsLimited = false;
    std::string  StartTime;
    std::string  EndTime;

    std::shared_ptr<Card> FeaturedCard = nullptr;

    nlohmann::json ToJson() const;
    static Banner FromJson(const nlohmann::json& j);
};
