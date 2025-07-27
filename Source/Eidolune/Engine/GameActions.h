#pragma once

#include <memory>
#include "Player.h"
#include "GameCard.h"
#include "TriggerObserver.h"
#include <memory>
#include "Target.h"

namespace GameActions {

    void PlayCard(Player* player, int index, std::shared_ptr<TriggerObserver> observer);
    void Attack(Player* attacker, Player* defender);
    void UseAbility(Player* player, Player* opponent);
    void StartTurn(Player* player);
    void EndTurn(Player* player);

    void ResolveCombat(std::shared_ptr<GameCard> attackerCard, GameCard* targetCard, Player* attacker, Player* defender);

    //void ResolveDamage(void* source, void* target, int amount, const std::string& type);
    void ResolveDamage(void* source, Target target, int amount, const std::string& type);
    
    void ResolveHeal(std::shared_ptr<GameCard> source, void* target, int amount);

    void ShowHand(Player* player);
    void ShowBoard(Player* player);
    void ShowPlayerState(Player* player);

    std::vector<Target> GetTargets(Player* player, Player* opponent, TargetSpec spec);
    Target ChooseTarget(Player* player, const std::vector<Target>& targets);
}
