#include "Gacha.h"
#include "Banner.h"
#include "PullTracker.h"
#include "CardRegistry.h"
#include "CharacterRegistry.h"
#include "UserDataUploader.h"
#include <iostream>
#include <random>

namespace {

float GetAdjustedRarity4Rate(int pullsSinceLastRare, int softPity, int hardPity, float baseRate) {
    if (pullsSinceLastRare < softPity) return baseRate;
    if (pullsSinceLastRare >= hardPity) return 1.0f;

    float increment = (1.0f - baseRate) / (hardPity - softPity);
    return baseRate + increment * (pullsSinceLastRare - softPity);
}

Rarity RollRarity(const Banner& banner, int pity, std::mt19937& rng) {
    float rarity4Rate = GetAdjustedRarity4Rate(pity, banner.SoftPityStart, banner.HardPity, banner.BaseRates.at(Rarity::LEGENDARY));
    float r = std::uniform_real_distribution<float>(0.0f, 1.0f)(rng);

    float accum = 0.0f;
    for (const auto& [rarity, rate] : banner.BaseRates) {
        float finalRate = (rarity == Rarity::LEGENDARY) ? rarity4Rate : rate;
        accum += finalRate;
        if (r < accum) return rarity;
    }
    return Rarity::COMMON;
}

std::shared_ptr<Card> PullOneCard(const Banner& banner, Rarity rarity, std::mt19937& rng) {
    std::vector<BannerCardEntry> candidates;
    for (const auto& entry : banner.Pool) {
        if (entry.Card->CardRarity == rarity) {
            candidates.push_back(entry);
        }
    }

    if (candidates.empty()) return nullptr;

    std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
    return candidates[dist(rng)].Card;
}

void PrintCardResult(const Card& card, int quantity, bool newCharacter) {
    std::string rarityStr;
    switch (card.CardRarity) {
        case Rarity::COMMON: rarityStr = "★★"; break;
        case Rarity::RARE: rarityStr = "★★★"; break;
        case Rarity::EPIC: rarityStr = "★★★★"; break;
        case Rarity::LEGENDARY: rarityStr = "★★★★★"; break;
    }

    std::cout << "- " << rarityStr << " " << card.Name;
    if (quantity > 1) std::cout << " (x" << quantity << ")";
    if (newCharacter) std::cout << " [New Character Unlocked!]";
    std::cout << "\n";
}

}

void Gacha::Run(std::shared_ptr<User> user, UserData& userData) {
    std::cout << "Select banner to pull from:\n";
    // Stubbed banner list for now
    Banner banner = LoadBannerById(1); // Placeholder
    GachaPullTracker& tracker = userData.GachaTrackers[banner.Id];

    std::cout << "Pull how many times? (1 or 10): ";
    int count;
    std::cin >> count;
    if (count != 1 && count != 10) count = 1;

    std::random_device rd;
    std::mt19937 rng(rd());

    for (int i = 0; i < count; ++i) {
        ++tracker.TotalPulls;
        ++tracker.PullsSinceLastRare;

        Rarity rolledRarity = RollRarity(banner, tracker.PullsSinceLastRare, rng);
        if (rolledRarity == Rarity::LEGENDARY) tracker.PullsSinceLastRare = 0;

        auto card = PullOneCard(banner, rolledRarity, rng);
        if (!card) continue;

        bool newCharacter = false;
        if (card->IsCharacterCard && card->CharacterRef) {
            int charId = card->CharacterRef->ID;
            auto existingChar = std::find_if(userData.Characters.begin(), userData.Characters.end(),
                [charId](const UserCharacterData& c) { return c.CharacterId == charId; });

            if (existingChar == userData.Characters.end()) {
                UserCharacterData charData = UserCharacterData::FromCharacter(card->CharacterRef.get());
                userData.SyncCharacter(charData);
                newCharacter = true;
            }
        }

        UserCardData userCard = UserCardData::FromCard(card.get());
        userData.SyncCard(userCard);
        PrintCardResult(*card, userCard.Quantity, newCharacter);
    }

    UserDataUploader::UploadUserData(user, userData);
}
