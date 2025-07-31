
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
#include "DeckValidator.h"
#include "../Definitions/DeckRestrictionDefinitions.h"
#include "../Loaders/SubtypeLoader.h"

#include <random>

#include "Init.h"


struct UserInfo {
    std::string Username;
    std::vector<std::shared_ptr<Deck>> ValidDecks;
};


std::pair<std::shared_ptr<Deck>, std::string> SelectDeckForUser(const std::vector<UserInfo>& users, const std::string& prompt) {
    std::cout << "\n🔍 " << prompt << "\n";
    for (size_t i = 0; i < users.size(); ++i) {
        std::cout << i << ": 👤 " << users[i].Username << "\n";
    }

    int userIndex;
    std::cout << "Enter user index: ";
    std::cin >> userIndex;

    const auto& user = users.at(userIndex);

    std::cout << "\n🎴 Valid Decks for " << user.Username << ":\n";
    for (size_t i = 0; i < user.ValidDecks.size(); ++i) {
        std::cout << i << ": " << user.ValidDecks[i]->Name << "\n";
    }

    int deckIndex;
    std::cout << "Choose deck index: ";
    std::cin >> deckIndex;
    std::cin.ignore();

    return { user.ValidDecks.at(deckIndex), user.Username };
}

void SubscribeCardTriggers(std::shared_ptr<GameCard> card, std::shared_ptr<TriggerObserver> observer) {

    if (card->Model->EffectBindings.empty()) {
        return;
    }

    for (const auto& originalBinding : card->Model->EffectBindings) {

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

        auto* triggerMeta = TriggerRegistry::Instance().GetInfo(triggerCode);
        if (!triggerMeta) {
            std::cout << "❌ No trigger metadata found for code: " << triggerCode << "\n";
            continue;
        }

        std::string eventName = triggerMeta->Event;

        binding->EventGameCard = card;

        std::function<void(const std::unordered_map<std::string, void*>)> effectToRegister;
        auto builder = TriggerBuilder::Build(binding);

        auto conditionPtr = binding->GetCondition();
        if (conditionPtr) {
            std::string conditionCode = conditionPtr->ToString();

            effectToRegister = [=](const std::unordered_map<std::string, void*>& ctx) {
                if (ConditionEvaluator::Evaluate(conditionCode, card, binding->GetValue().value_or(0))) {

                    builder(ctx);
                } 
            };
        } else {
            effectToRegister = builder;
        }

        observer->Subscribe(eventName, effectToRegister);
    }

}

std::vector<UserInfo> BuildUserDeckInfoList() {
    std::vector<UserInfo> users;

    auto allUsers = UserRegistry::Instance().GetAll();
    auto allDecks = DeckRegistry::Instance().GetAll();

    for (const auto& [_, user] : allUsers) {
        UserInfo userInfo;
        userInfo.Username = user->Username;

        for (const auto& [_, deck] : allDecks) {
            if (deck->Owner && deck->Owner->Id == user->Id) {
                auto result = DeckValidator::Validate(deck);
                if (result.IsPlayable) {
                    userInfo.ValidDecks.push_back(deck);
                } else {
                    std::cout << "❌ Deck '" << deck->Name << "' is invalid:\n";
                    for (const auto& issue : result.Issues) {
                        std::cout << " - " << issue << "\n";
                    }
                }
            }
        }

        if (!userInfo.ValidDecks.empty()) {
            users.push_back(userInfo);
        }
    }

    return users;
}

void SetupGameState(GameState& game, std::shared_ptr<Player> p1, std::shared_ptr<Player> p2) {
    game.Players = { p1, p2 };
    p1->Opponent = p2.get();
    p2->Opponent = p1.get();

    auto observer = game.GetTriggerObserver();

    for (auto& player : game.Players) {
        for (auto& deckCard : player->DeckRef->DeckCards) {
            for (int i = 0; i < deckCard->Quantity; ++i) {
                auto card = std::make_shared<GameCard>(deckCard->CardRef);
                card->Owner = player.get();
                card->Zone = CardZone::DECK;
                SubscribeCardTriggers(card, observer);
                deckCard->GameCardCopies.push_back(card);
            }
        }

        player->DrawPile.clear();
        for (auto& deckCard : player->DeckRef->DeckCards) {
            for (auto& gameCard : deckCard->GameCardCopies) {
                player->DrawPile.push_back(gameCard);
            }
        }

        std::shuffle(player->DrawPile.begin(), player->DrawPile.end(), std::mt19937{ std::random_device{}() });

        for (int i = 0; i < 3; ++i) {
            DrawCard(nullptr, Target::FromPlayer(player.get()), 1);
        }
    }
}

void GameEngine::Run() {
    std::cout << "🎮 Starting Test Game\n";

    EidoluneInit::RegisterAll();
    EidoluneInit::LoadAll();
    /*
    RegisterConditionFunctions();
    RegisterEffectFunctions();
    RegisterDeckRestrictions();

    TriggerLoader::LoadAll();
    EffectLoader::LoadAll();
    ConditionLoader::LoadAll();
    SubtypeLoader::LoadAll();
    CharacterLoader::LoadAll();
    CardLoader::LoadAll();
    CardEffectBindingLoader::LoadAll();
    UserLoader::LoadAll();
    DeckLoader::LoadAll();
    DeckCardLoader::LoadAll();
    */

    auto users = BuildUserDeckInfoList();
    if (users.empty()) {
        std::cout << "❌ No users with valid decks found.\n";
        return;
    }

    auto [deck1, user1] = SelectDeckForUser(users, "Select Player 1");
    auto [deck2, user2] = SelectDeckForUser(users, "Select Player 2");

    std::cout << "Selected Deck 1: " << deck1->Name << " by " << user1 << "\n";
    std::cout << "Selected Deck 2: " << deck2->Name << " by " << user2 << "\n";

    auto player1 = std::make_shared<Player>(deck1, 0);
    auto player2 = std::make_shared<Player>(deck2, 0);

    GameState game;
    SetupGameState(game, player1, player2);

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
                GameActions::PlayCard(current.get(), index, game.GetTriggerObserver());
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