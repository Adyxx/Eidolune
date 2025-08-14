#include "GameActions.h"
#include "TriggerObserver.h"
#include <iostream>
#include <algorithm>
#include "Character.h"
#include "Deck.h"
#include "Position.h"
#include "CharacterClass.h"

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
        std::cout << "\n🛡️ Board for " << player->GetName() << ":\n";

        for (int row = 0; row < Player::BoardHeight; ++row) {
            for (int col = 0; col < Player::BoardWidth; ++col) {
                auto& slot = player->GridBoard[row][col];
                int tileID = row * Player::BoardWidth + col + 1;

                if (slot)
                    std::cout << "[" << tileID << ": " << slot->GetName() << "]";
                else
                    std::cout << "[" << tileID << ": ---]";
            }
            std::cout << "\n";
        }
    }


    void ShowPlayerState(Player* player) {
        std::cout << "\n🧝 Player: " << player->GetName()
                << " | ❤️ Health: " << player->Health
                << " | 🔋 Energy: " << player->Energy;
                
        if (player->HasClassBloodbound) {
            std::cout << " | 🩸 BloodEcho: " << player->BloodboundData->BloodEcho;
        }        
                
        std::cout << "\n";

        ShowHand(player);
        ShowBoard(player);
    }

    std::vector<Target> GetTargets(Player* player, Player* opponent, TargetSpec spec) {
        std::vector<Target> targets;

        auto addBoardCards = [&](Player* p) {
            for (int row = 0; row < Player::BoardHeight; ++row)
                for (int col = 0; col < Player::BoardWidth; ++col)
                    if (p->GridBoard[row][col])
                        targets.push_back(Target::FromCard(p->GridBoard[row][col].get()));
        };

        switch (spec) {
            case TargetSpec::ENEMY_BOARD:
                addBoardCards(opponent);
                break;

            case TargetSpec::ENEMY_HERO:
                targets.push_back(Target::FromPlayer(opponent));
                break;

            case TargetSpec::ENEMY_BOARD_OR_HERO:
                addBoardCards(opponent);
                targets.push_back(Target::FromPlayer(opponent));
                break;

            case TargetSpec::FRIENDLY_BOARD:
                addBoardCards(player);
                break;

            case TargetSpec::FRIENDLY_HERO:
            case TargetSpec::SELF:
                targets.push_back(Target::FromPlayer(player));
                break;

            case TargetSpec::SELF_EQUIPPED_UNIT:
                /*
                if (card->AttachedTo) {
                    return TargetResult{ card->AttachedTo.lock(), nullptr };
                }
                */
                break;
            
            case TargetSpec::ANY:
                addBoardCards(player);
                addBoardCards(opponent);
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
                            << " (" << gc->CurrentAttack()
                            << "/" << gc->CurrentHealth() << ")\n";
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
        int cost = card->GetCost();

        if (player->Energy < cost) {
            std::cout << "🚫 Not enough energy to play " << card->GetName() << "\n";
            return;
        }

        // Remove from hand and set ownership
        player->Hand.erase(player->Hand.begin() + index);
        card->Owner = player;

        switch (card->Model->Type) {
            case CardType::SPELL: {
                std::cout << "✨ " << player->GetName() << " casts " << card->GetName() << "\n";
                card->Zone = CardZone::GRAVEYARD;
                player->Graveyard.push_back(card);
                break;
            }

            case CardType::UNIT: {
                std::cout << "Enter position to place unit (1–14): ";
                int posID;
                std::cin >> posID;
                std::cin.ignore();

                if (posID < 1 || posID > 14) {
                    std::cout << "🚫 Invalid position.\n";
                    return;
                }

                auto pos = FromID(posID);
                auto& slot = player->GridBoard[pos.row][pos.col];
                if (slot) {
                    std::cout << "🚫 That tile is already occupied.\n";
                    return;
                }

                card->Zone = CardZone::BOARD;
                slot = card;
                std::cout << "▶️ " << player->GetName() << " plays " << card->GetName()
                        << " to position " << posID << "\n";
                break;
            }

            case CardType::ASSET: {
                std::cout << "Enter position of unit to attach asset (1–14): ";
                int posID;
                std::cin >> posID;
                std::cin.ignore();

                if (posID < 1 || posID > 14) {
                    std::cout << "🚫 Invalid position.\n";
                    return;
                }

                auto pos = FromID(posID);
                auto& targetSlot = player->GridBoard[pos.row][pos.col];
                if (!targetSlot || targetSlot->Model->Type != CardType::UNIT) {
                    std::cout << "🚫 No valid unit in that position to attach asset.\n";
                    return;
                }

                targetSlot->Attachments.push_back(card);
                card->AttachedTo = targetSlot;
                card->Zone = CardZone::BOARD; // or CardZone::ATTACHED if you track separately

                std::cout << "🔗 " << player->GetName() << " attaches " << card->GetName()
                        << " to " << targetSlot->GetName() << "\n";
                break;
            }

            case CardType::RITE: {
                // Rites occupy the board but do not attack
                std::cout << "Enter position to place rite (1–14): ";
                int posID;
                std::cin >> posID;
                std::cin.ignore();

                if (posID < 1 || posID > 14) {
                    std::cout << "🚫 Invalid position.\n";
                    return;
                }

                auto pos = FromID(posID);
                auto& slot = player->GridBoard[pos.row][pos.col];
                if (slot) {
                    std::cout << "🚫 That tile is already occupied.\n";
                    return;
                }

                card->Zone = CardZone::BOARD;
                slot = card;
                std::cout << "📜 " << player->GetName() << " plays rite " << card->GetName()
                        << " to position " << posID << "\n";
                break;
            }

            case CardType::EVENT: {
                // Events don't go to board — register effects and track duration
                std::cout << "📢 " << player->GetName() << " plays event " << card->GetName() << "\n";
                card->Zone = CardZone::GRAVEYARD; // or EVENT_ZONE if you track them
                player->ActiveEvents.push_back(card); // custom container
                break;
            }

            default:
                std::cout << "🚫 Unknown card type.\n";
                return;
        }

        // Deduct energy
        player->Energy -= cost;
        std::cout << "🔋 Energy left: " << player->Energy << "\n";

        // Emit card_played trigger
        observer->Emit("card_played", { {"source", card.get()}, {"owner", player} });
    }

    void Attack(Player* attacker, Player* defender) {
        std::vector<std::shared_ptr<GameCard>> validAttackers;

        for (int row = 0; row < Player::BoardHeight; ++row)
            for (int col = 0; col < Player::BoardWidth; ++col) {
                auto& card = attacker->GridBoard[row][col];
                if (card && !card->SummoningSickness && !card->Tapped && card->GetPower().value_or(0) > 0 && card->Model->Type == CardType::UNIT)
                    validAttackers.push_back(card);
            }


        if (validAttackers.empty()) {
            std::cout << "❌ No valid attackers.\n";
            return;
        }

        std::cout << "\n⚔️ Available attackers:\n";
        for (size_t i = 0; i < validAttackers.size(); ++i) {
            auto& card = validAttackers[i];
            std::cout << "  " << i << ": " << card->GetName()
                    << " (" << card->CurrentAttack()
                    << "/" << card->CurrentHealth() << ")\n";
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
        attacker->Observer->Emit("card_attacked", { {"source", attackerCard.get()}, {"owner", attacker} });

        if (targetCard) {
            std::cout << "⚔️ " << attackerCard->GetName() << " attacks " << targetCard->GetName() << "\n";
            GameActions::ResolveDamage(attackerCard.get(), Target::FromCard(targetCard), attackerCard->CurrentAttack(), "combat");
            GameActions::ResolveDamage(targetCard, Target::FromCard(attackerCard.get()), targetCard->CurrentAttack(), "combat");
        } else {
            std::cout << "⚔️ " << attackerCard->GetName() << " attacks " << defender->GetName() << "\n";
            GameActions::ResolveDamage(attackerCard.get(), Target::FromPlayer(defender), attackerCard->CurrentAttack(), "combat");
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

                // Emit "on_damage_received" for target
                if (targetCard->Owner) {
                    targetCard->Owner->Observer->Emit("damage_received", {
                        {"target", targetCard},
                        {"amount", &amount},
                        {"source", source}
                    });
                }

                // Emit "on_damage_dealt" for source
                if (source) {
                    if (auto* sourceCard = static_cast<GameCard*>(source)) {
                        if (sourceCard->Owner) {
                            sourceCard->Owner->Observer->Emit("damage_dealt", {
                                {"source", sourceCard},
                                {"amount", &amount},
                                {"target", targetCard}
                            });
                        }
                    }
                }

                // Death check
                if (targetCard->CurrentHealth() <= 0) {
                    std::cout << "💀 " << targetCard->GetName() << " is destroyed!\n";
                    Player* owner = targetCard->Owner;
                    targetCard->Zone = CardZone::GRAVEYARD;

                    if (owner) {
                        owner->Observer->Emit("card_died", { {"source", targetCard}, {"owner", owner} });
                        owner->Graveyard.push_back(std::shared_ptr<GameCard>(targetCard));
                        owner->RemoveCardFromBoard(targetCard);
                        owner->RemoveAllAurasFromSource(targetCard->Id);
                    }
                }
                break;
            }

            case Target::Type::PLAYER: {
                auto* player = static_cast<Player*>(target.ptr);
                std::cout << "🟥 Dealing " << amount << " to player: " << player->GetName() << "\n";

                // Emit "on_damage_received" for player
                if (player) {
                    player->Observer->Emit("damage_received", {
                        {"target", player},
                        {"amount", &amount},
                        {"source", source}
                    });
                }

                if (player->HasClassBloodbound) {
                    player->BloodboundData->BloodEcho += amount; // lost HP
                }

                // Emit "on_damage_dealt" for source
                if (source) {
                    if (auto* sourceCard = static_cast<GameCard*>(source)) {
                        if (sourceCard->Owner) {
                            sourceCard->Owner->Observer->Emit("damage_dealt", {
                                {"source", sourceCard},
                                {"amount", &amount},
                                {"target", player}
                            });
                        }
                    }
                }

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

    void ResolveHeal(void* source, Target target, int amount, const std::string& type) {
        /*
            FOR LATER... for now just healHero func

                
        */
    }

    void UseAbility(Player* player, Player* opponent) {
        std::cout << "❌ Ability system not yet implemented.\n";
    }

    void StartTurn(Player* player, std::shared_ptr<TriggerObserver> observer) {
        player->StartTurn();

        observer->Emit("turn_started", {
            { "turn_player", player }, 
            { "owner", player },      
            { "source", nullptr }   
        });

        if (player->DeckRef && player->DeckRef->MainCharacter && player->DeckRef->MainCharacter->ClassLogic) {
            player->DeckRef->MainCharacter->ClassLogic->OnTurnStart(player);
        }
    }

    void EndTurn(Player* player, std::shared_ptr<TriggerObserver> observer) {
        player->EndTurn();

        observer->Emit("turn_ended", {
            { "turn_player", player }, 
            { "owner", player },    
            { "source", nullptr }  
        });
    }
}
