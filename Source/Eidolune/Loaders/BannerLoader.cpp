#include "BannerLoader.h"
#include "../Http/ApiClient.h"
#include "../Registry/BannerRegistry.h"
#include "../Registry/CardRegistry.h"
#include <iostream>

static BannerType ParseBannerType(const std::string& s) {
    if (s == "CARD") return BannerType::CARD;
    if (s == "EQUIPMENT") return BannerType::EQUIPMENT;
    throw std::runtime_error("Unknown banner type: " + s);
}

static CurrencyType ParseCurrencyType(const std::string& s) {
    if (s == "STANDARD_PULL") return CurrencyType::STANDARD_PULL;
    if (s == "LIMITED_PULL") return CurrencyType::LIMITED_PULL;
    throw std::runtime_error("Unknown currency type: " + s);
}

void BannerLoader::LoadAll() {
    std::cout << "🎯 Loading Banners from API...\n";
    auto jsonArray = ApiClient::Get("banners/active/");

    for (const auto& j : jsonArray) {
        try {
            Banner banner;
            banner.Id = j["id"];
            banner.Name = j["name"];
            banner.Type = ParseBannerType(j["type"]);
            banner.PullCurrency = ParseCurrencyType(j["pull_currency"]);
            banner.HardPity = j["hard_pity"];
            banner.SoftPityStart = j["soft_pity_start"];
            banner.IsLimited = j["is_limited"];
            banner.StartTime = j["start_time"];
            banner.EndTime = j["end_time"];

            banner.BaseRates[Rarity::COMMON] = 0.67f;
            banner.BaseRates[Rarity::RARE] = 0.23f;
            banner.BaseRates[Rarity::EPIC] = 0.09f;
            banner.BaseRates[Rarity::LEGENDARY] = 0.01f;

            // Optional featured legendary (nullable)
            if (!j["featured_legendary"].is_null()) {
                int featuredId = j["featured_legendary"]["id"];
                auto featuredCard = CardRegistry::Instance().Get(featuredId);
                if (featuredCard) {
                    banner.FeaturedCard = featuredCard;
                } else {
                    std::cerr << "⚠️ Featured card ID " << featuredId << " not found\n";
                }
            }

            // Banner items
            for (const auto& item : j["items"]) {
                const auto& cardData = item["card"];
                int cardId = cardData["id"];
                auto card = CardRegistry::Instance().Get(cardId);
                if (!card) {
                    std::cerr << "⚠️ Banner item references unknown card ID " << cardId << "\n";
                    continue;
                }

                BannerCardEntry bce;
                bce.Card = card;
                bce.IsRateUp = item.value("is_featured", false);
                banner.Pool.push_back(bce);
            }

            BannerRegistry::Instance().Register(banner.Id, std::make_shared<Banner>(banner));
        } catch (const std::exception& e) {
            std::cerr << "❌ Error loading banner: " << e.what() << "\n";
        }
    }
}
