
#include "GameEngine.h"
#include "GameActions.h"
#include "../Definitions/EffectDefinitions.h"
#include <iostream>

void GameEngine::Run() {
    std::cout << "🎮 Starting Test Game\n";

    GameState game;
    game.Players = { player1, player2 };

    for (auto& player : game.Players) {
        for (int i = 0; i < 3; ++i) {
            DrawCard(nullptr, player.get(), 1);
        }
    }

    while (!game.GameOver) {
        auto current = game.GetCurrentPlayer();
        std::cout << "\n=== 🕒 " << current->GetName() << "'s Turn ===\n";

        GameActions::StartTurn(current.get());

        bool turnEnded = false;
        while (!turnEnded) {
            std::cout << "\n>> Command (play <i> / attack / end): ";
            std::string input;
            std::getline(std::cin, input);

            if (input.substr(0, 4) == "play") {
                int index = std::stoi(input.substr(5));
                GameActions::PlayCard(current.get(), index);
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
