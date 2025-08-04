#include "GameActions.h"
#include "TriggerObserver.h"
#include <iostream>
#include <algorithm>
#include "Character.h"


namespace GameActions {

    void ShowHand(Player* player) {
        std::cout << "\n📜 " << player->GetName() << "'s hand:\n";
        for (size_t i = 0; i < player->Hand.size(); ++i) {
            auto& card = player->Hand[i];
            std::cout << "  " << i << ": " << card->GetName()
                      << " (Cost: " << card->GetCost()
                      << ", Power: " << card->GetPower().value_or(0)
                      << ", HP: " << card->GetHealth().value_or(0)
                      << ", id: " << card->Id << "\n";
        }
    }

    void ShowBoard(Player* player) {
        std::cout << "🛡️ Board: ";
        for (auto& card : player->Board) {
            std::cout << card->GetName() << " ";
        }
        std::cout << "\n";
    }

    void ShowPlayerState(Player* player) {
    std::cout << "\n🧝 Player: " << player->GetName()
              << " | ❤️ Health: " << player->Health
              << " | 🔋 Energy: " << player->Energy << "\n";

    ShowHand(player);
    ShowBoard(player);
    }

    std::vector<Target> GetTargets(Player* player, Player* opponent, TargetSpec spec) {
        std::vector<Target> targets;

        switch (spec) {
            case TargetSpec::ENEMY_BOARD:
                for (auto& c : opponent->Board) targets.push_back(Target::FromCard(c.get()));
                break;

            case TargetSpec::ENEMY_HERO:
                targets.push_back(Target::FromPlayer(opponent));
                break;

            case TargetSpec::ENEMY_BOARD_OR_HERO:
                for (auto& c : opponent->Board) targets.push_back(Target::FromCard(c.get()));
                targets.push_back(Target::FromPlayer(opponent));
                break;

            case TargetSpec::FRIENDLY_BOARD:
                for (auto& c : player->Board) targets.push_back(Target::FromCard(c.get()));
                break;

            case TargetSpec::FRIENDLY_HERO:
            case TargetSpec::SELF:
                targets.push_back(Target::FromPlayer(player));
                break;

            case TargetSpec::ANY:
                for (auto& c : player->Board) targets.push_back(Target::FromCard(c.get()));
                for (auto& c : opponent->Board) targets.push_back(Target::FromCard(c.get()));
                targets.push_back(Target::FromPlayer(player));
                targets.push_back(Target::FromPlayer(opponent));
                break;

            default:
                break;
        }

        return targets;
    }


    Target ChooseTarget(Player* player, const std::vector<Target>& targets) {
        if (targets.empty()) {
            std::cout << "❌ No valid targets.\n";
            return Target{};
        }

        if (targets.size() == 1) {
            return targets.front();
        }

        std::cout << "\n🎯 Choose a target:\n";
        for (size_t i = 0; i < targets.size(); ++i) {
            auto& target = targets[i];

            switch (target.type) {
                case Target::Type::CARD: {
                    auto* gc = static_cast<GameCard*>(target.ptr);
                    std::cout << "  " << i << ": " << gc->GetName()
                            << " (" << gc->GetPower().value_or(0)
                            << "/" << gc->GetHealth().value_or(0) << ")\n";
                    break;
                }
                case Target::Type::PLAYER: {
                    auto* pl = static_cast<Player*>(target.ptr);
                    std::cout << "  " << i << ": Player - " << pl->GetName()
                            << " (HP: " << pl->Health << ")\n";
                    break;
                }
                default:
                    std::cout << "  " << i << ": Unknown target\n";
                    break;
            }
        }

        std::cout << "Select target ID: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice < 0 || choice >= static_cast<int>(targets.size())) {
            std::cout << "❌ Invalid target.\n";
            return Target{};
        }

        return targets[choice];
    }



    void PlayCard(Player* player, int index, std::shared_ptr<TriggerObserver> observer) {
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
            card->Zone = CardZone::GRAVEYARD;
            player->Graveyard.push_back(card);
        } else {
            card->Zone = CardZone::BOARD;
            player->Board.push_back(card);
            std::cout << "▶️ " << player->GetName() << " plays " << card->GetName() << " to the board\n";
        }

        player->Energy -= card->GetCost();
        std::cout << "🔋 Energy left: " << player->Energy << "\n";

        std::cout << "Emitting event <card_played> " << "\n";
        observer->Emit("card_played", { {"source", card.get()},{"owner", player} });

    }

    void PlayCardDirect(std::shared_ptr<GameCard> card, std::shared_ptr<TriggerObserver> observer) {
        Player* player = card->Owner;

        if (!player) {
            std::cerr << "❌ Cannot play card — no owner assigned.\n";
            return;
        }

        if (player->Energy < card->GetCost()) {
            std::cout << "🚫 Not enough energy to play " << card->GetName() << "\n";
            return;
        }

        if (card->Model->Type == CardType::SPELL) {
            std::cout << "✨ " << player->GetName() << " casts " << card->GetName() << "\n";
            card->Zone = CardZone::GRAVEYARD;
            player->Graveyard.push_back(card);
        } else {
            card->Zone = CardZone::BOARD;
            player->Board.push_back(card);
            std::cout << "▶️ " << player->GetName() << " plays " << card->GetName() << " to the board\n";
        }

        player->Energy -= card->GetCost();
        std::cout << "🔋 Energy left: " << player->Energy << "\n";

        observer->Emit("card_played", { {"source", card.get()}, {"owner", player} });
    }

    void Attack(Player* attacker, Player* defender) {
        std::vector<std::shared_ptr<GameCard>> validAttackers;

        for (auto& card : attacker->Board) {
            if (!card->SummoningSickness && !card->Tapped && card->GetPower().value_or(0) > 0) {
                validAttackers.push_back(card);
            }
        }

        if (validAttackers.empty()) {
            std::cout << "❌ No valid attackers.\n";
            return;
        }

        std::cout << "\n⚔️ Available attackers:\n";
        for (size_t i = 0; i < validAttackers.size(); ++i) {
            auto& card = validAttackers[i];
            std::cout << "  " << i << ": " << card->GetName()
                    << " (" << card->GetPower().value_or(0)
                    << "/" << card->GetHealth().value_or(0) << ")\n";
        }

        std::cout << "Select attacker ID: ";
        int idx;
        std::cin >> idx;
        std::cin.ignore();

        if (idx < 0 || idx >= static_cast<int>(validAttackers.size())) {
            std::cout << "❌ Invalid attacker choice.\n";
            return;
        }

        auto attackerCard = validAttackers[idx];

        auto potentialTargets = GetTargets(attacker, defender, TargetSpec::ENEMY_BOARD_OR_HERO);
        Target target = ChooseTarget(attacker, potentialTargets);

        if (target.type == Target::Type::NONE || target.ptr == nullptr) {
            std::cout << "❌ No valid target selected.\n";
            return;
        }

        if (target.type == Target::Type::CARD) {
            auto* targetCard = static_cast<GameCard*>(target.ptr);
            ResolveCombat(attackerCard, targetCard, attacker, defender);
        } else if (target.type == Target::Type::PLAYER) {
            auto* targetPlayer = static_cast<Player*>(target.ptr);
            ResolveCombat(attackerCard, nullptr, attacker, targetPlayer);
        } else {
            std::cout << "❌ Unknown target type.\n";
        }
    }

   void ResolveCombat(std::shared_ptr<GameCard> attackerCard, GameCard* targetCard, Player* attacker, Player* defender) {
        if (targetCard) {
            std::cout << "⚔️ " << attackerCard->GetName() << " attacks " << targetCard->GetName() << "\n";
            GameActions::ResolveDamage(attackerCard.get(), Target::FromCard(targetCard), attackerCard->GetPower().value_or(0), "combat");
            GameActions::ResolveDamage(targetCard, Target::FromCard(attackerCard.get()), targetCard->GetPower().value_or(0), "combat");
        } else {
            std::cout << "⚔️ " << attackerCard->GetName() << " attacks " << defender->GetName() << "\n";
            GameActions::ResolveDamage(attackerCard.get(), Target::FromPlayer(defender), attackerCard->GetPower().value_or(0), "combat");
        }

        attackerCard->Tapped = true;
    }

    void ResolveDamage(void* source, Target target, int amount, const std::string& type) {
        if (!target.ptr) return;

        switch (target.type) {
            case Target::Type::CARD: {
                auto* targetCard = static_cast<GameCard*>(target.ptr);
                std::cout << "🟥 Dealing " << amount << " to card: " << targetCard->GetName() << "\n";
                targetCard->DamageTaken += amount;
                if (targetCard->DamageTaken >= targetCard->GetHealth().value_or(0)) {
                    std::cout << "💀 " << targetCard->GetName() << " is destroyed!\n";
                    auto* owner = targetCard->Owner;
                    if (owner) {
                        auto& board = owner->Board;
                        auto it = std::find_if(board.begin(), board.end(),
                            [&](const std::shared_ptr<GameCard>& c) { return c.get() == targetCard; });
                        if (it != board.end()) {
                            owner->Graveyard.push_back(*it);
                            board.erase(it);
                        }
                    }
                }
                break;
            }
            case Target::Type::PLAYER: {
                auto* player = static_cast<Player*>(target.ptr);
                std::cout << "🟥 Dealing " << amount << " to player: " << player->GetName() << "\n";
                player->Health -= amount;
                if (player->Health <= 0) {
                    std::cout << "☠️ " << player->GetName() << " has died!\n";
                }
                break;
            }
            default:
                std::cout << "❌ Unknown target type.\n";
                break;
        }

        if (source) {
            if (auto* sourceCard = static_cast<GameCard*>(source)) {
                std::cout << "[Source] " << sourceCard->GetName() << "\n";
            } else {
                std::cout << "[Source] Unknown\n";
            }
        }
    }

    

    void UseAbility(Player* player, Player* opponent) {
        std::cout << "❌ Ability system not yet implemented.\n";
    }

    void StartTurn(Player* player) {
        player->StartTurn();

        if (player->DeckRef && player->DeckRef->MainCharacter && player->DeckRef->MainCharacter->ClassLogic) {
            player->DeckRef->MainCharacter->ClassLogic->OnTurnStart(player);
        }
    }

    void EndTurn(Player* player) {
        player->EndTurn();
    }

}
