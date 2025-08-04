#include "Gacha.h"
#include "Banner.h"
#include "PullTracker.h"
#include "CardRegistry.h"
#include "CharacterRegistry.h"
#include "UserDataUploader.h"
#include <iostream>
#include <random>
#include "BannerRegistry.h"

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
    //std::cout << "RARITY........... " << rarity4Rate << "\n";
    float accum = 0.0f;
    for (const auto& [rarity, rate] : banner.BaseRates) {
        float finalRate = (rarity == Rarity::LEGENDARY) ? rarity4Rate : rate;
        accum += finalRate;
        if (r < accum) return rarity;
    }
    return Rarity::COMMON;
}

std::shared_ptr<Card> PullOneCard(const Banner& banner, Rarity rarity, std::mt19937& rng) {
    std::vector<std::shared_ptr<Card>> candidates;

    for (const auto& entry : banner.Pool) {
        if (entry.Card->CardRarity == rarity) {
            // Exclude legendary if it's the featured card and will be handled separately
            if (rarity == Rarity::LEGENDARY && entry.Card == banner.FeaturedCard) continue;
            candidates.push_back(entry.Card);
        }
    }

    if (candidates.empty()) return nullptr;

    std::uniform_int_distribution<size_t> dist(0, candidates.size() - 1);
    return candidates[dist(rng)];
}


}

void Gacha::Run(std::shared_ptr<User> user, UserData& userData) {
    const auto& banners = BannerRegistry::Instance().GetAll();

    if (banners.empty()) {
        std::cout << "❌ No active banners found.\n";
        return;
    }

    std::cout << "Select banner to pull from:\n";
    for (const auto& [id, banner] : banners) {
        std::cout << "  [" << id << "] " << banner->Name;
        if (banner->IsLimited) std::cout << " [LIMITED]";
        std::cout << "\n";
    }

    int selectedId;
    std::cout << "Enter banner ID: ";
    std::cin >> selectedId;

    auto selectedBanner = BannerRegistry::Instance().Get(selectedId);
    if (!selectedBanner) {
        std::cout << "❌ Invalid banner ID.\n";
        return;
    }

    std::string trackerKey = (selectedBanner->IsLimited ? "LIMITED_" : "STANDARD_");
    trackerKey += (selectedBanner->Type == BannerType::CARD ? "CARD" : "EQUIPMENT");

    GachaPullTracker& tracker = userData.GachaTrackers[trackerKey];

    std::cout << "Pull how many times? (1 or 10): ";
    int count;
    std::cin >> count;
    if (count != 1 && count != 10) count = 1;

    std::random_device rd;
    std::mt19937 rng(rd());

    for (int i = 0; i < count; ++i) {
        ++tracker.TotalPulls;
        ++tracker.PullsSinceLastRare;

        Rarity rolledRarity = RollRarity(*selectedBanner, tracker.PullsSinceLastRare, rng);
        std::shared_ptr<Card> card;

        if (rolledRarity == Rarity::LEGENDARY) {
            tracker.PullsSinceLastRare = 0;

            bool useFeatured = false;

            if (tracker.NextLegendaryGuaranteedFeatured) {
                useFeatured = true;
                tracker.NextLegendaryGuaranteedFeatured = false;
            } else {
                useFeatured = (std::uniform_int_distribution<>(0, 1)(rng) == 0);
                tracker.NextLegendaryGuaranteedFeatured = !useFeatured;
            }

            if (useFeatured && selectedBanner->FeaturedCard) {
                card = selectedBanner->FeaturedCard;
            } else {
                // Get non-featured legendaries from pool
                std::vector<std::shared_ptr<Card>> nonFeaturedLegendaries;
                for (const auto& entry : selectedBanner->Pool) {
                    if (entry.Card->CardRarity == Rarity::LEGENDARY &&
                        entry.Card != selectedBanner->FeaturedCard) {
                        nonFeaturedLegendaries.push_back(entry.Card);
                    }
                }

                if (!nonFeaturedLegendaries.empty()) {
                    std::uniform_int_distribution<size_t> dist(0, nonFeaturedLegendaries.size() - 1);
                    card = nonFeaturedLegendaries[dist(rng)];
                } else if (selectedBanner->FeaturedCard) {
                    // Fallback if no others exist
                    card = selectedBanner->FeaturedCard;
                }
            }
        } else {
            card = PullOneCard(*selectedBanner, rolledRarity, rng);
        }

        if (!card) {
            std::cout << "- No card found for rarity " << static_cast<int>(rolledRarity) << "\n";
            continue;
        }

        bool newCharacter = false;
        if (card->IsCharacterCard && card->CharacterRef) {
            int charId = card->CharacterRef->ID;
            auto existingChar = std::find_if(
                userData.Characters.begin(), userData.Characters.end(),
                [charId](const UserCharacterData& c) { return c.CharacterId == charId; });

            if (existingChar == userData.Characters.end()) {
                UserCharacterData charData = UserCharacterData::FromCharacter(card->CharacterRef.get());
                userData.SyncCharacter(charData);
                newCharacter = true;
            }
        }

        UserCardData userCard = UserCardData::FromCard(card.get());
        userData.SyncCard(userCard);

        std::string rarityStr;
        switch (card->CardRarity) {
            case Rarity::COMMON: rarityStr = "★★"; break;
            case Rarity::RARE: rarityStr = "★★★"; break;
            case Rarity::EPIC: rarityStr = "★★★★"; break;
            case Rarity::LEGENDARY: rarityStr = "★★★★★"; break;
        }

        std::cout << "- " << rarityStr << " " << card->Name;
        if (card == selectedBanner->FeaturedCard) std::cout << " [FEATURED]";
        if (userCard.Quantity > 1) std::cout << " (x" << userCard.Quantity << ")";
        if (newCharacter) std::cout << " [New Character Unlocked!]";
        std::cout << " (Pity: " << tracker.PullsSinceLastRare << "/80)";
        std::cout << "\n";
    }
        

    UserDataUploader::UploadUserData(user, userData);
}