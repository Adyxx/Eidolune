#include "UserDataLoader.h"

#include "../Http/ApiClient.h"
#include "../Registry/DeckRegistry.h"
#include "../Registry/CharacterRegistry.h"
#include "../Data/UserCharacterData.h"

#include <iostream>
#include "../Classes/SpecialistClass.h"
#include "../Registry/CardRegistry.h"

UserData UserDataLoader::LoadAllForUser(std::shared_ptr<User> user) {
    UserData userData;

    if (!user) {
        std::cerr << "❌ Cannot load user data: null user\n";
        return userData;
    }

    std::cout << "📦 Fetching data for user '" << user->Username << "'...\n";

    json dataJson = ApiClient::Get("users/" + std::to_string(user->Id) + "/data/");
    userData = UserData::FromJson(dataJson);
    userData.UserId = user->Id;

    // 🔄 Resolve owned cards
    int cardsResolved = 0;
    for (auto& userCard : userData.Cards) {
        auto card = CardRegistry::Instance().Get(userCard.CardId);
        if (card) {
            userCard.ResolvedCard = card;
            ++cardsResolved;
        } else {
            std::cerr << "❌ Owned card ID " << userCard.CardId << " not found in CardRegistry.\n";
        }   
        if (userCard.ResolvedCard) {
            std::cout << "🃏 Owned card: " << userCard.ResolvedCard->Name
                      << " (ID: " << userCard.CardId << ", Qty: " << userCard.Quantity << ")\n";
        }
    }
    std::cout << "✅ Resolved " << cardsResolved << " owned cards.\n";

    // 🔄 Register decks
    int deckCount = 0;
    for (const auto& deck : userData.Decks) {
        if (deck && deck->Owner) {
            DeckRegistry::Instance().Register(deck->ID, deck);
            ++deckCount;
        } else {
            std::cerr << "⚠️ Skipping invalid deck entry.\n";
        }
    }
    std::cout << "✅ Registered " << deckCount << " decks.\n";

    // 🔄 Resolve characters
    int charResolved = 0;
    for (auto& charData : userData.Characters) {
        auto character = CharacterRegistry::Instance().Get(charData.CharacterId);
        if (character) {
            charData.ResolvedCharacter = character;
            std::cout << "✅ Resolved character '" << character->Name << "' (ID: " << character->ID << ")\n";
            ++charResolved;
        } else {
            std::cerr << "❌ Character ID " << charData.CharacterId << " not found in CharacterRegistry.\n";
        }
    }
    std::cout << "✅ Resolved " << charResolved << " characters.\n";

    return userData;
}


void UserDataLoader::ApplyUserCharacterDataToCharacters(UserData& userData) {
    for (auto& userCharData : userData.Characters) {
        auto character = CharacterRegistry::Instance().Get(userCharData.CharacterId);
        if (!character) {
            std::cerr << "❌ Character ID " << userCharData.CharacterId << " not found to apply user data.\n";
            continue;
        }


        userCharData.ResolvedCharacter = character;

        if (character->Class == CharacterClassType::SPECIALIST && character->ClassLogic) {
            auto specialist = std::dynamic_pointer_cast<SpecialistClass>(character->ClassLogic);
            if (specialist) {
                auto& bag = specialist->GetBag(character.get());
                bag.slots.clear();
                bag.slots.resize(userCharData.Consumables.size());

                for (size_t i = 0; i < userCharData.Consumables.size(); ++i) {
                    int cardId = userCharData.Consumables[i];
                    std::shared_ptr<Card> card = nullptr;

                    if (cardId != -1) {
                        card = CardRegistry::Instance().Get(cardId);
                        if (!card) {
                            std::cerr << "⚠️ Consumable card ID " << cardId << " not found.\n";
                        }
                    }

                    if (card) {
                        int defaultUses = (card->AuxilaryType == AuxiliaryCardType::CONSUMABLE) ? 1 : 0;
                        auto instance = std::make_shared<ConsumableInstance>(card, defaultUses);
                        bag.slots[i].item = instance;
                        std::cout << "✅ Equipped '" << card->Name << "' to Specialist bag slot " << i << "\n";
                    } else {
                        bag.slots[i].item = nullptr;
                    }
                }
            }
        }

        // You can later apply userCharData.Level, EquippedItems, etc. to relevant systems
        // For now, maybe log them for debug
        std::cout << "🧬 Applied user character data: level " << userCharData.Level << ", ID " << userCharData.CharacterId << "\n";
    }
}
