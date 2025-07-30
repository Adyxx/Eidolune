
#include "GameEngine.h"
#include "GameActions.h"
#include "../Definitions/EffectDefinitions.h"
#include "../Definitions/TriggerDefinitions.h"
#include "../Definitions/ConditionDefinitions.h"
#include "../Registry/CharacterClassRegistry.h"
#include <iostream>
#include "../Core/Player.h"
#include "../Loaders/UserLoader.h"
#include "../Loaders/CardLoader.h"  
#include "../Loaders/DeckLoader.h"
#include "../Loaders/CharacterLoader.h"
#include "../Core/Card.h"   
#include "../Core/DeckCard.h"
#include "TriggerObserver.h"
#include "../Registry/TriggerRegistry.h"
#include "ConditionEvaluator.h"
#include "Trigger.h"
#include "TriggerBuilder.h"
#include "Condition.h"
#include "../Core/CardEffectBinding.h"
#include "../Registry/UserRegistry.h"
#include "../Registry/DeckRegistry.h"
#include "../Loaders/DeckCardLoader.h"
#include "../Loaders/CardEffectBindingLoader.h"
#include "../Loaders/ConditionLoader.h"
#include "../Loaders/EffectLoader.h"
#include "../Loaders/TriggerLoader.h"

#include <random>

struct DeckInfo {
    std::string Name;
    std::vector<std::shared_ptr<DeckCard>> DeckCards;
};

struct UserInfo {
    std::string Username;
    std::vector<DeckInfo> Decks;
};


std::pair<DeckInfo, std::string> SelectDeckForUser(const std::vector<UserInfo>& users, const std::string& prompt) {
    std::cout << "\n🔍 " << prompt << "\n";
    for (size_t i = 0; i < users.size(); ++i) {
        std::cout << i << ": 👤 " << users[i].Username << "\n";
    }

    int userIndex;
    std::cout << "Enter user index: ";
    std::cin >> userIndex;

    const auto& user = users.at(userIndex);

    std::cout << "\n🎴 Decks for " << user.Username << ":\n";
    for (size_t i = 0; i < user.Decks.size(); ++i) {
        std::cout << i << ": " << user.Decks[i].Name << "\n";
    }

    int deckIndex;
    std::cout << "Choose deck index: ";
    std::cin >> deckIndex;
    std::cin.ignore();

    return { user.Decks.at(deckIndex), user.Username };
}



std::shared_ptr<Player> CreatePlayerFromDeck(const DeckInfo& deckInfo, const std::string& username) {
    auto deck = std::make_shared<Deck>(nullptr, deckInfo.Name, nullptr);

    std::cout << "Creating deck: " << deck->Name << " for user: " << username << "\n";
    std::cout << "Deck contains " << deckInfo.DeckCards.size() << " cards.\n";

    for (const auto& deckCard : deckInfo.DeckCards) {
        auto newDeckCard = std::make_shared<DeckCard>(deck, deckCard->CardRef, deckCard->Quantity);
        deck->DeckCards.push_back(newDeckCard);
    }

    auto player = std::make_shared<Player>(deck, 0);

    return player;
}

void SubscribeCardTriggers(std::shared_ptr<GameCard> card, std::shared_ptr<TriggerObserver> observer) {
    std::cout << "\n🔔 SubscribeCardTriggers ENTERED for card: " << card->GetName() << "\n";

    if (card->Model->EffectBindings.empty()) {
        std::cout << "⚠️ No effect bindings found on card model: " << card->Model->Name << "\n";
        return;
    }

    for (const auto& originalBinding : card->Model->EffectBindings) {
        std::cout << "🔍 Found EffectBinding. Attempting to clone and subscribe...\n";

        auto binding = std::make_shared<CardEffectBinding>(*originalBinding);

        if (!binding) {
            std::cout << "❌ Failed to copy original binding.\n";
            continue;
        }

        auto triggerPtr = binding->GetTrigger();
        if (!triggerPtr) {
            std::cout << "⚠️ Skipping binding: no trigger found.\n";
            continue;
        }

        std::string triggerCode = triggerPtr->ScriptReference;
        std::cout << "📛 Trigger code: " << triggerCode << "\n";

        auto* triggerMeta = TriggerRegistry::Instance().GetInfo(triggerCode);
        if (!triggerMeta) {
            std::cout << "❌ No trigger metadata found for code: " << triggerCode << "\n";
            continue;
        }

        std::string eventName = triggerMeta->Event;
        std::cout << "📡 Subscribing to event: " << eventName << "\n";

        binding->EventGameCard = card;

        std::cout << "🧱 Building trigger handler from binding...\n";
        std::function<void(const std::unordered_map<std::string, void*>)> effectToRegister;
        auto builder = TriggerBuilder::Build(binding);

        auto conditionPtr = binding->GetCondition();
        if (conditionPtr) {
            std::string conditionCode = conditionPtr->ToString();
            std::cout << "🧪 Binding has condition: " << conditionCode << "\n";

            effectToRegister = [=](const std::unordered_map<std::string, void*>& ctx) {
                if (ConditionEvaluator::Evaluate(conditionCode, card, binding->GetValue().value_or(0))) {
                    std::cout << "✅ Condition met. Executing effect.\n";
                    builder(ctx);
                } else {
                    std::cout << "🚫 Condition not met. Skipping effect.\n";
                }
            };
        } else {
            std::cout << "☑️ No condition found. Effect will always trigger.\n";
            effectToRegister = builder;
        }

        observer->Subscribe(eventName, effectToRegister);
        std::cout << "✅ Subscribed " << card->GetName()
                  << " to trigger '" << eventName << "' successfully.\n";
    }

    std::cout << "🔚 Finished subscribing triggers for card: " << card->GetName() << "\n\n";
}

std::vector<UserInfo> BuildUserDeckInfoList() {
    std::vector<UserInfo> users;

    std::cout << "[Debug] Building user deck info list...\n";
    auto allUsers = UserRegistry::Instance().GetAll();
    auto allDecks = DeckRegistry::Instance().GetAll();

    std::cout << "[Debug] Total users in registry: " << allUsers.size() << "\n";
    std::cout << "[Debug] Total decks in registry: " << allDecks.size() << "\n";

    for (const auto& [_, user] : allUsers) {
        std::cout << "[Debug] Processing user: " << user->Username << " (Id: " << user->Id << ")\n";

        UserInfo userInfo;
        userInfo.Username = user->Username;

        for (const auto& [_, deck] : allDecks) {
            if (deck->Owner && deck->Owner->Id == user->Id) {
                std::cout << "  [Debug] Found deck for user: " << deck->Name << "\n";

                DeckInfo deckInfo;
                deckInfo.Name = deck->Name;
                for (const auto& deckCard : deck->DeckCards) {
                    deckInfo.DeckCards.push_back(deckCard);
                }
                userInfo.Decks.push_back(deckInfo);
            }
        }

        if (userInfo.Decks.empty()) {
            std::cout << "  [Debug] No decks found for user " << user->Username << "\n";
        }

        users.push_back(userInfo);
    }
    std::cout << "[Debug] Finished building user deck info list.\n";
    return users;
}

void GameEngine::Run() {
    std::cout << "🎮 Starting Test Game\n";
    GameState game;

    RegisterConditionFunctions();
    RegisterEffectFunctions();

    // 🔹 1. Load metadata-based types
    TriggerLoader::LoadAll();
    EffectLoader::LoadAll();
    ConditionLoader::LoadAll();

    // 🔹 2. Load static content
    CharacterLoader::LoadAll();
    CardLoader::LoadAll();
    CardEffectBindingLoader::LoadAll();

    // 🔹 3. Load dynamic/player-specific content
    UserLoader::LoadAll();
    DeckLoader::LoadAll();

    // 🔹 4. DeckCards reference both Decks and Cards
    DeckCardLoader::LoadAll();
    


    auto usersFromRegistry = UserRegistry::Instance().GetAll();
    std::cout << "[Debug] Users loaded from registry:\n";
    for (const auto& [_, user] : usersFromRegistry) {
        std::cout << " - " << user->Username << " (Id: " << user->Id << ")\n";
    }

    auto decksFromRegistry = DeckRegistry::Instance().GetAll();
    std::cout << "[Debug] Decks loaded from registry:\n";
    for (const auto& [_, deck] : decksFromRegistry) {
        std::cout << " - " << deck->Name << ", owner Id: " << (deck->Owner ? std::to_string(deck->Owner->Id) : "null") << "\n";
    }


    std::vector<UserInfo> users = BuildUserDeckInfoList();

    auto [deck1, user1] = SelectDeckForUser(users, "Select Player 1");
    auto [deck2, user2] = SelectDeckForUser(users, "Select Player 2");

    std::cout << "Selected Deck 1: " << deck1.Name << " for User: " << user1 << "\n";
    std::cout << "Selected Deck 2: " << deck2.Name << " for User: " << user2 << "\n";

    auto player1 = CreatePlayerFromDeck(deck1, user1);
    auto player2 = CreatePlayerFromDeck(deck2, user2);

    std::cout << "Created Player 1: " << player1->GetName() << "\n";
    std::cout << "Created Player 2: " << player2->GetName() << "\n";

    game.Players = { player1, player2 };
    player1->Opponent = player2.get();
    player2->Opponent = player1.get();

    std::cout << " *********************** " << "\n";
    for (auto& player : game.Players) {
        // Create GameCard instances per DeckCard
        for (auto& deckCard : player->DeckRef->DeckCards) {
            std::cout << " 🃏 Card: " << deckCard->CardRef->Name << ", Quantity: " << deckCard->Quantity << "\n";
            for (int i = 0; i < deckCard->Quantity; ++i) {
                std::cout << "🃏 Adding card to deck: " << deckCard->CardRef->Name << "\n";
                auto card = std::make_shared<GameCard>(deckCard->CardRef);
                card->Owner = player.get();
                card->Zone = CardZone::DECK;
                SubscribeCardTriggers(card, game.Observer);
                deckCard->GameCardCopies.push_back(card);
            }
        }

        // Now flatten all the GameCardCopies into player's DrawPile
        player->DrawPile.clear();
        for (auto& deckCard : player->DeckRef->DeckCards) {
            for (auto& gameCard : deckCard->GameCardCopies) {
                player->DrawPile.push_back(gameCard);
            }
        }

        // Then shuffle player's DrawPile here
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(player->DrawPile.begin(), player->DrawPile.end(), g);
    }



    for (auto& player : game.Players) {
        for (int i = 0; i < 3; ++i) {
            DrawCard(nullptr, Target::FromPlayer(player.get()), 1);
        }
    }

    auto observer = game.GetTriggerObserver();
    while (!game.GameOver) {
        auto current = game.GetCurrentPlayer();
        std::cout << "\n=== 🕒 " << current->GetName() << "'s Turn ===\n";
        
        GameActions::StartTurn(current.get());
        GameActions::ShowPlayerState(current.get());
        
        bool turnEnded = false;
        while (!turnEnded) {
            std::cout << "\n>> Command (play <i> / attack / end): ";
            std::string input;
            std::getline(std::cin, input);

            if (input.substr(0, 4) == "play") {
                int index = std::stoi(input.substr(5));
                 GameActions::PlayCard(current.get(), index, observer);
            } else if (input == "attack") {
                GameActions::Attack(current.get(), game.GetOpponent().get());
            } else if (input == "end") {
                GameActions::EndTurn(current.get());
                turnEnded = true;
            } else {
                std::cout << "❓ Unknown command.\n";
            }
        }

        game.NextTurn();
    }

    std::cout << "\n🏁 Game Over!\n";
}


GameEngine::GameEngine() {
    
}