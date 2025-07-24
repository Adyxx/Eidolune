#pragma once

#include <memory>
#include "Player.h"
#include "GameCard.h"

namespace GameActions {

    void PlayCard(Player* player, int index);
    void Attack(Player* attacker, Player* defender);
    void UseAbility(Player* player, Player* opponent);
    void StartTurn(Player* player);
    void EndTurn(Player* player);


    void ResolveCombat(std::shared_ptr<GameCard> attackerCard, std::shared_ptr<GameCard> targetCard, Player* attacker, Player* defender);
    void ResolveDamage(std::shared_ptr<GameCard> source, GameCard* targetCard, int amount, const std::string& type);
    void ResolveDamage(std::shared_ptr<GameCard> source, Player* playerTarget, int amount, const std::string& type);
    void ResolveHeal(std::shared_ptr<GameCard> source, void* target, int amount);

    void ShowHand(Player* player);
    void ShowBoard(Player* player);
    Player* ChooseTarget(Player* player, const std::string& targetSpec);
}
