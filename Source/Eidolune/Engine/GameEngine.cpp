
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
#include "../Core/Card.h"   
#include "../Core/DeckCard.h"
#include "TriggerObserver.h"
#include "../Registry/TriggerRegistry.h"
#include "ConditionEvaluator.h"
#include "Trigger.h"
#include "TriggerBuilder.h"
#include "Condition.h"


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

    for (const auto& card : deckInfo.Cards) {
        auto deckCard = std::make_shared<DeckCard>(deck, card, 1);
        deck->DeckCards.push_back(deckCard);
    }

    auto player = std::make_shared<Player>(deck, 0);

    return player;
}

void SubscribeCardTriggers(std::shared_ptr<GameCard> card, std::shared_ptr<TriggerObserver> observer) {
    for (auto& originalBinding : card->Model->EffectBindings) {
        auto binding = std::make_shared<CardEffectBinding>(*originalBinding);
        auto triggerPtr = binding->GetTrigger();
        if (!triggerPtr) continue;

        std::string triggerCode = triggerPtr->ScriptReference;
        auto* triggerMeta = TriggerRegistry::Get(triggerCode);
        if (!triggerMeta) continue;

        auto eventName = triggerMeta->Event;

        binding->EventGameCard = card; 

        auto builder = TriggerBuilder::Build(binding);

        std::function<void(const std::unordered_map<std::string, void*>)> effectToRegister;
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
        std::cout << "🧠 Subscribed " << card->GetName() << " to trigger '" << eventName << "'\n";
    }
}

void GameEngine::Run() {
    
    RegisterTriggerDefinitions();
    InitEffectRegistry();
    RegisterConditionDefinitions();

    std::cout << "🎮 Starting Test Game\n";
    GameState game;
    
    auto allCards = CardLoader::LoadAll("Content/Cards/cards.json");
    auto users = UserLoader::LoadUsers("Content/Users/", allCards);


    auto [deck1, user1] = SelectDeckForUser(users, "Select Player 1");
    auto [deck2, user2] = SelectDeckForUser(users, "Select Player 2");

    auto player1 = CreatePlayerFromDeck(deck1, user1);
    auto player2 = CreatePlayerFromDeck(deck2, user2);

    game.Players = { player1, player2 };

    player1->Opponent = player2.get();
    player2->Opponent = player1.get();

    for (auto& player : game.Players) {
        for (auto& deckCard : player->DeckRef->DeckCards) {
            for (int i = 0; i < deckCard->Quantity; ++i) {
                auto card = std::make_shared<GameCard>(deckCard->CardRef);
                card->Owner = player.get();
                card->Zone = CardZone::DECK;
                SubscribeCardTriggers(card, game.Observer);
                deckCard->GameCardCopies.push_back(card);
            }
        }
    }

    for (auto& player : game.Players) {
        for (int i = 0; i < 3; ++i) {
            //DrawCard(nullptr, player.get(), 1);
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