#include "Banner.h"
#include "CardRegistry.h"
#include "Card.h"


Banner LoadBannerById(int id) {
    auto allCards = CardRegistry::Instance().GetAll();
    Banner banner;
    banner.Id = id;
    banner.Name = "Celestial Dawn Banner";
    banner.Type = BannerType::CARD;
    banner.PullCurrency = CurrencyType::STANDARD_PULL;
    banner.BaseRates = {
        {Rarity::COMMON, 0.65f},
        {Rarity::RARE, 0.25f},
        {Rarity::EPIC, 0.09f},
        {Rarity::LEGENDARY, 0.01f}
    };
    banner.HardPity = 80;
    banner.SoftPityStart = 60;

    for (const auto& [_, card] : allCards) {
        if (!card) continue;
        if (card->AuxilaryType != AuxiliaryCardType::NONE) continue;
        if (!card->IsPullable()) continue; 
        banner.Pool.push_back({card, 1.0f, false});
    }

    return banner;
}
