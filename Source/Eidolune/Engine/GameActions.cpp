#include "GameActions.h"
#include "TriggerObserver.h"
#include <iostream>
#include <algorithm>

namespace GameActions {

    void ShowHand(Player* player) {
        std::cout << "\n📜 " << player->GetName() << "'s hand:\n";
        for (size_t i = 0; i < player->Hand.size(); ++i) {
            auto& card = player->Hand[i];
            std::cout << "  " << i << ": " << card->GetName()
                      << " (Cost: " << card->GetCost()
                      << ", Power: " << card->GetPower().value_or(0)
                      << ", HP: " << card->GetHealth().value_or(0) << ")\n";
        }
    }

    void ShowBoard(Player* player) {
        std::cout << "🛡️ Board: ";
        for (auto& card : player->Board) {
            std::cout << card->GetName() << " ";
        }
        std::cout << "\n";
    }

    void PlayCard(Player* player, int index) {
        if (index < 0 || index >= static_cast<int>(player->Hand.size())) {
            std::cout << "🚫 Invalid card index.\n";
            return;
        }

        auto card = player->Hand[index];

        if (player->Energy < card->GetCost()) {
            std::cout << "🚫 Not enough energy to play " << card->GetName() << "\n";
            return;
        }

        player->Hand.erase(player->Hand.begin() + index);
        card->Owner = player;

        if (card->Model->Type == CardType::SPELL) {
            std::cout << "✨ " << player->GetName() << " casts " << card->GetName() << "\n";
            card->Zone = "graveyard";
            player->Graveyard.push_back(card);
        } else {
            card->Zone = "board";
            player->Board.push_back(card);
            std::cout << "▶️ " << player->GetName() << " plays " << card->GetName() << " to the board\n";
        }

        player->Energy -= card->GetCost();
        std::cout << "🔋 Energy left: " << player->Energy << "\n";

        // triggerObserver->Emit("card_played", card, player);
    }

    void Attack(Player* attacker, Player* defender) {
        std::vector<std::shared_ptr<GameCard>> attackers;

        for (auto& card : attacker->Board) {
            if (!card->SummoningSickness && !card->Tapped && card->GetPower().value_or(0) > 0) {
                attackers.push_back(card);
            }
        }

        if (attackers.empty()) {
            std::cout << "❌ No valid attackers.\n";
            return;
        }

        std::cout << "\n⚔️ Available attackers:\n";
        for (size_t i = 0; i < attackers.size(); ++i) {
            auto& card = attackers[i];
            std::cout << "  " << i << ": " << card->GetName()
                      << " (" << card->GetPower().value_or(0)
                      << "/" << card->GetHealth().value_or(0) << ")\n";
        }

        std::cout << "Select attacker ID: ";
        int idx;
        std::cin >> idx;
        std::cin.ignore();

        if (idx < 0 || idx >= static_cast<int>(attackers.size())) {
            std::cout << "❌ Invalid attacker choice.\n";
            return;
        }

        auto attackerCard = attackers[idx];

        std::cout << "Target: (0) Enemy Hero, (1) Enemy Board Card\n";
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "0") {
            ResolveCombat(attackerCard, nullptr, attacker, defender);
        } else if (choice == "1" && !defender->Board.empty()) {
            for (size_t i = 0; i < defender->Board.size(); ++i) {
                auto& targetCard = defender->Board[i];
                std::cout << "  " << i << ": " << targetCard->GetName()
                          << " (" << targetCard->GetPower().value_or(0)
                          << "/" << targetCard->GetHealth().value_or(0) << ")\n";
            }
            std::cout << "Select target ID: ";
            int tidx;
            std::cin >> tidx;
            std::cin.ignore();
            if (tidx >= 0 && tidx < static_cast<int>(defender->Board.size())) {
                ResolveCombat(attackerCard, defender->Board[tidx], attacker, defender);
            }
        } else {
            std::cout << "❌ Invalid choice.\n";
        }
    }

    void ResolveCombat(std::shared_ptr<GameCard> attackerCard, std::shared_ptr<GameCard> targetCard, Player* attacker, Player* defender) {
        std::cout << "START Checkpoint resolve combat \n";

        if (targetCard) {
            std::cout << "⚔️ " << attackerCard->GetName() << " attacks " << targetCard->GetName() << "\n";
            ResolveDamage(attackerCard, targetCard.get(), attackerCard->GetPower().value_or(0), "combat");
            ResolveDamage(targetCard, attackerCard.get(), targetCard->GetPower().value_or(0), "combat");
        } else {
            std::cout << "⚔️ " << attackerCard->GetName() << " attacks " << defender->GetName() << "\n";
            ResolveDamage(attackerCard, defender, attackerCard->GetPower().value_or(0), "combat");
        }

        attackerCard->Tapped = true;
        std::cout << "END Checkpoint resolve combat \n";
    }

    void ResolveDamage(std::shared_ptr<GameCard> source, GameCard* targetCard, int amount, const std::string& type) {
        std::cout << "START Checkpoint resolve damage \n";
        if (!targetCard) return;

        targetCard->DamageTaken += amount;
        if (targetCard->DamageTaken >= targetCard->GetHealth().value_or(0)) {
            std::cout << "💀 " << targetCard->GetName() << " is destroyed!\n";
            Player* owner = targetCard->Owner;
            if (owner) {
                auto& board = owner->Board;

                auto it = std::find_if(board.begin(), board.end(),
                    [&](const std::shared_ptr<GameCard>& c) { return c.get() == targetCard; });

                if (it != board.end()) {
                    owner->Graveyard.push_back(*it);
                    board.erase(it);
                } else {
                    std::cout << "❌ Could not find targetCard on board\n";
                }
            }
        }
         std::cout << "END Checkpoint resolve damage \n";
    }

    void ResolveDamage(std::shared_ptr<GameCard> source, Player* playerTarget, int amount, const std::string& type) {
        std::cout << "START Checkpoint resolve combat \n";
        if (!playerTarget) return;

        playerTarget->Health -= amount;
        if (playerTarget->Health <= 0) {
            std::cout << "☠️ " << playerTarget->GetName() << " has died!\n";
        }
        std::cout << "END Checkpoint resolve combat \n";
    }


    void UseAbility(Player* player, Player* opponent) {
        std::cout << "❌ Ability system not yet implemented.\n";
    }

    void StartTurn(Player* player) {
        player->StartTurn();
    }

    void EndTurn(Player* player) {
        player->EndTurn();
    }

}
